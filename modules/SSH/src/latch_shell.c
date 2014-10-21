/*
 * This library checks status of Latch and exec a shell.
 * Copyright (C) 2013 Eleven Paths

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "config.h"
#include "../../../lib/latch.h"
#include "../../../lib/util.h"
 #include "../../../lib/drop_privs.h"



/* Usage: latch-shell [-o operation][-s][-f config_file][-a accounts_file]
 * Must be silent mode because of SFTP 
 */


static int exec_shell(){
    if (getenv("SSH_ORIGINAL_COMMAND") != NULL) {
        return execl(getenv("SHELL"), getenv("SHELL"), "-c", getenv("SSH_ORIGINAL_COMMAND"), NULL);
    }else{
        return execl(getenv("SHELL"), getenv("SHELL"), "-c", getenv("SHELL"), NULL);
    }
}

static int latch_shell_status(const char *pAccountId, int defaultOption) {
    int res = 0;
    char *buffer = NULL; 

    if (pAccountId == NULL) {
        return 0;
    }

    buffer = status(pAccountId);
          
    if(buffer == NULL || strcmp(buffer,"") == 0) {
        free(buffer);
        return defaultOption;
    }

    if (strstr(buffer, "\"status\":\"off\"") != NULL){
        res = 1;
    } else if (strstr(buffer, "\"status\":\"on\"") != NULL){
        res = 0;
    }

    free(buffer);
    return res;
}

static int latch_shell_operation_status(const char *pAccountId, const char *pOperationId, int defaultOption) {
    int res = 0;
    char *buffer = NULL; 

    if (pAccountId == NULL) {
        return 0;
    }

    buffer = operationStatus(pAccountId, pOperationId);
        
    if(buffer == NULL || strcmp(buffer,"") == 0) {
        free(buffer);
        return defaultOption;
    }

    if (strstr(buffer, "\"status\":\"off\"") != NULL){
        res = 1;
    } else if (strstr(buffer, "\"status\":\"on\"") != NULL){
        res = 0;
    }

    free(buffer);
    return res;
}


int main(int argc, char **argv) {

    int fperms = 0;
    int aperms = 0;
    int sflag = 0;
    char *fvalue = NULL;
    char *avalue = NULL;
    char *ovalue = NULL;
    int index;
    int c;
    int error = 0;
    const char *pAccountId = NULL;
    const char* pUsername = NULL;                
    const char* pSecretKey = NULL;
    const char* pAppId = NULL;
    const char* pHost = NULL;
    const char* pTimeout = NULL;
    const char *pOperationId = NULL;
    char* pDefaultOption = NULL;
    char *buffer;    
    int timeout = 2;
    int res = 0;
    int default_option = 0;
    FILE *f;

    opterr = 0;
  

    while ((c = getopt (argc, argv, "so:f:a:")) != -1) {
      switch(c) {
          case 's':
              sflag = 1;
              break;
          case 'o':
              ovalue = optarg;
              break;
          case 'f':
              fvalue = optarg;
              break;
          case 'a':
              avalue = optarg;
              break;
          case '?':
          default:
              error++;
      }
    }

    if (argc < 2) {
        return exec_shell();
    }

    if (optind < argc){
        return exec_shell();
    }

    if (error) {
        return exec_shell();
    }
   
    pUsername = get_user_name();
    if (pUsername == NULL) {
        return exec_shell();
    }
     
    if (avalue == NULL) {
        avalue = DEFAULT_LATCH_ACCOUNTS_FILE;
        aperms = 1;
    } else if (access(avalue, R_OK) != 0) {
        return 1;
    }
      
    if (fvalue == NULL) {
        fvalue = DEFAULT_LATCH_CONFIG_FILE;
        fperms = 1;
    } else if (access(fvalue, R_OK) != 0) {
        return 1;
    }

    if (!fperms && drop_privileges(0)) {
        return 1;
    }

    pAppId = getConfig(APP_ID_LENGTH, "app_id", fvalue);
    pSecretKey = getConfig(SECRET_KEY_LENGTH, "secret_key", fvalue);
   
    if(pAppId == NULL || pSecretKey == NULL || strcmp(pAppId, "") == 0 || strcmp(pSecretKey, "") == 0){
        return exec_shell();
    }

    if(ovalue && (pOperationId = getConfig(OPERATION_ID_LENGTH, ovalue, fvalue) == NULL)) {
        return 1;
    }
     
    pDefaultOption = (char*)getConfig(DEFAULT_OPTION_MAX_LENGTH, "action", fvalue);
    if (pDefaultOption == NULL) {
        pDefaultOption = malloc(4 + 1); 
        memset(pDefaultOption, 0, 4 + 1);
        strncpy(pDefaultOption, "open", 4);
    } else if (strcmp(pDefaultOption,"open") != 0 && strcmp(pDefaultOption,"close") != 0){
        pDefaultOption = realloc(pDefaultOption, 4 + 1);
        memset(pDefaultOption, 0, 4 + 1);
        strncpy(pDefaultOption, "open", 4);
    }

    if (strcmp(pDefaultOption,"open") == 0) {
        default_option = 0;
    } else {
        default_option = 1;
    }
    free((char*)pDefaultOption);

    pHost = getConfig(MAX_SIZE, "latch_host", fvalue);
    if (pHost == NULL) {
        pHost = malloc(LATCH_API_HOST_LENGTH + 1);
        memset((char*)pHost, 0, LATCH_API_HOST_LENGTH + 1);
        strncpy((char*)pHost, LATCH_API_HOST, LATCH_API_HOST_LENGTH);
    }

    pTimeout = getConfig(TIMEOUT_MAX_LENGTH, "timeout", fvalue);
    if (pTimeout == NULL || ((timeout = atoi(pTimeout)) < TIMEOUT_MIN) || timeout > TIMEOUT_MAX) {
        timeout = 2;
    }
    free((char*)pTimeout);
 
    if (!aperms && drop_privileges(0)) {
        return 1;
    } 

    if (aperms && !fperms) {
        restore_privileges();
    }

    pAccountId = getAccountId(pUsername, avalue);

    if (drop_privileges(1)) {
        return 1;
    }

    init(pAppId, pSecretKey);
    setHost(pHost);
    setTimeout(timeout);
           
    if (sflag) {
        res = latch_shell_status(pAccountId, default_option);
    } else if (ovalue) {
        res = latch_shell_operation_status(pAccountId, pOperationId, default_option);
    } 

    free((char*)pAccountId);
    free((char*)pAppId);
    free((char*)pSecretKey);
    free((char*)pHost);
    free((char*)pOperationId);
   
    if (! res) {
        res = exec_shell();
    }

    return res;
}
