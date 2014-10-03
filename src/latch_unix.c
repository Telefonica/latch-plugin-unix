/*
 * This library implements all functions of Latch API.
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
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <errno.h>
#include "config.h"

#include "latch_unix.h"
#include "../lib/latch.h"
#include "../lib/util.h"
#include "../lib/drop_privs.h"
#include "../lib/charset.h"



void print_usage() {
    printf("%s\n", LATCH_USAGE);
}

void print_version() {
    printf("%s\n", LATCH_VERSION);
}


static int latch_pair(const char *username, const char *pAccountId, const char *accountsFile, int aperms, char *pairingCode) {
    int res = 0;
    char *acc_id = NULL;
    char *buffer = NULL; 
    char *pstr = NULL;

    if (pAccountId != NULL) {
        fprintf(stderr, ALREADY_PAIRED_$USER_MSG, username);
        return 1;
    }

    if (! validCode(pairingCode)) {
        fprintf(stderr, "%s\n", INVALID_TOKEN_FORMAT_MSG);
        return 1;
    }

    buffer = pair(pairingCode);

    if (buffer == NULL || strcmp(buffer,"") == 0) {
        fprintf(stderr, "%s\n", CONNECTION_SERVER_ERROR_MSG);
        free(buffer);
        return 1;
    }

    if ((pstr = strstr(buffer, "accountId")) != NULL) {
        fprintf(stdout, "%s %s\n", PAIRING_SUCCESS_MSG, username);
        pstr += 12;
        acc_id = malloc(ACCOUNT_ID_LENGTH + 1);
        strncpy(acc_id, pstr, ACCOUNT_ID_LENGTH);
        acc_id[ACCOUNT_ID_LENGTH] = '\0';

        if (aperms && restore_privileges()) {
            fprintf(stderr, "%s\n", RESTORE_PRIVS_ERROR_MSG);
        }

        if (appendAccountId(username, acc_id, accountsFile) == -1) {
            fprintf(stderr, "%s %s\n", WRITE_ACC_FILE_ERROR_MSG, accountsFile);
            res = 1;
        }
    } else if (strstr(buffer, "\"error\":{\"code\":102") != NULL) {
        fprintf(stderr, "%s\n", LATCH_ERROR_102_MSG);
        res = 1;
    } else if (strstr(buffer, "\"error\":{\"code\":109") != NULL) {
        fprintf(stderr, "%s\n", LATCH_ERROR_109_MSG);
        res = 1;
    } else if (strstr(buffer, "\"error\":{\"code\":206") != NULL) {
        fprintf(stderr, "%s\n", LATCH_ERROR_206_MSG);
        res = 1;
    } else {
        fprintf(stderr, "%s %s\n", PAIRING_ERROR_DEFAULT_MSG, username);
        res = 1;
    }

    free(acc_id);
    free(buffer);
    return res;
}

static int latch_unpair(const char *username, const char *pAccountId, const char *accountsFile, int aperms) {
    int res = 0;
    char *buffer = NULL; 

    if (pAccountId == NULL) {
        fprintf(stderr, NOT_PAIRED_$USER_MSG, username);
        return 1;
    }

    if (aperms && restore_privileges()) {
        printf("%s\n", RESTORE_PRIVS_ERROR_MSG);
    } 

    if (deleteAccountId(username, accountsFile) == -1) {
        fprintf(stderr, "%s %s\n", WRITE_ACC_FILE_ERROR_MSG, accountsFile);
        free((char*)pAccountId);
        return 1;
    }

    fprintf(stdout, UNPAIRING_SUCCESS_$USER_MSG, username);
    if (countAccountId(pAccountId, accountsFile) == 0) {
        if (drop_privileges(1)) {
            fprintf(stderr, "%s\n", DROP_PRIVS_ERROR_MSG);
        }
        buffer = unpair(pAccountId);
        free(buffer);
    }

    return res;
}

static int latch_status(const char *username, const char *pAccountId) {
    int res = 0;
    char *buffer = NULL; 

    if (pAccountId == NULL) {
        fprintf(stderr, NOT_PAIRED_$USER_MSG, username);
        return 1;
    }

    fprintf(stdout, CHECK_STATUS_$USER_MSG, username);
    buffer = status(pAccountId);

    if(buffer == NULL || strcmp(buffer,"") == 0) {
        fprintf(stderr, "%s\n", CONNECTION_SERVER_ERROR_MSG);
        free(buffer);
        return 1;
    }

    if (strstr(buffer, "\"status\":\"off\"") != NULL){
        fprintf (stderr, "%s\n", LATCH_STATUS_OFF_MSG);
    } else if (strstr(buffer, "\"status\":\"on\"") != NULL){
        fprintf (stderr, "%s\n", LATCH_STATUS_ON_MSG);
    } else if (strstr(buffer, "\"error\":{\"code\":102") != NULL) {
        fprintf(stderr, "%s\n", LATCH_ERROR_102_MSG);
        res = 1;
    } else if (strstr(buffer, "\"error\":{\"code\":109") != NULL) {
        fprintf(stderr, "%s\n", LATCH_ERROR_109_MSG);
        res = 1;
    } else if (strstr(buffer, "\"error\":{\"code\":201") != NULL) {
        fprintf(stderr, LATCH_ERROR_201_$USER_MSG, username);
        res = 1;
    } else {
        fprintf(stderr, "%s\n", STATUS_ERROR_DEFAULT_MSG);
        res = 1;
    }

    free(buffer);
    return res;
}

static int latch_operation_status(const char *username, const char *pAccountId, const char *pOperationId) {
    int res = 0;
    char *buffer = NULL; 

    if (pAccountId == NULL) {
        fprintf(stderr, NOT_PAIRED_$USER_MSG, username);
        return 1;
    }

    fprintf(stdout, CHECK_STATUS_$USER_MSG, username);
    buffer = operationStatus(pAccountId, pOperationId);
          
    if(buffer == NULL || strcmp(buffer,"") == 0) {
        fprintf(stderr, "%s\n", CONNECTION_SERVER_ERROR_MSG);
        free(buffer);
        return 1;
    }

    if (strstr(buffer, "\"status\":\"off\"") != NULL){
        fprintf (stdout, "%s\n", LATCH_STATUS_OFF_MSG);
    } else if (strstr(buffer, "\"status\":\"on\"") != NULL){
        fprintf (stdout, "%s\n", LATCH_STATUS_ON_MSG);
    } else if (strstr(buffer, "\"error\":{\"code\":102") != NULL) {
        fprintf(stderr, "%s\n", LATCH_ERROR_102_MSG);
        res = 1;
    } else if (strstr(buffer, "\"error\":{\"code\":109") != NULL) {
        fprintf(stderr, "%s\n", LATCH_ERROR_109_MSG);
        res = 1;
    } else if (strstr(buffer, "\"error\":{\"code\":301") != NULL) {
        fprintf(stderr, LATCH_ERROR_301_$USER_MSG, username);
        res = 1;
    } else {
        fprintf(stderr, "%s\n", STATUS_ERROR_DEFAULT_MSG);
        res = 1;
    }

    free(buffer);
    return res;
}


int main(int argc, char **argv) {

	int fperms = 0;
	int aperms = 0;
    int hflag = 0;
    int vflag = 0;
    int uflag = 0;
    int sflag = 0;
    char *pvalue = NULL;
    char *fvalue = NULL;
    char *avalue = NULL;
    char *ovalue = NULL;
    int index = 0;
    int c;
    const char *pAccountId = NULL;
    const char* pUsername = NULL;                
    const char *pSecretKey = NULL;
    const char *pAppId = NULL;
    const char *pHost = NULL;
    const char *pTimeout = NULL; 
    const char *pOperationId = NULL;  
    int timeout = 2;
    int res = 0;
    FILE *f;

    opterr = 0;
  
    if (argc < 2) {
        print_usage();
        return 1;
    }

    while ((c = getopt (argc, argv, "hvp:uso:f:a:")) != -1) {
      switch(c) {
          case 'h':
          hflag = 1;
          break;
          case 'v':
          vflag = 1;
          break;
          case 'p':
          pvalue = optarg;
          break;
          case 'u':
          uflag = 1;
          break;
          case 's':
          sflag = 1;
          break;
          case 'f':
          fvalue = optarg;
          break;
          case 'o':
          ovalue = optarg;
          break;
          case 'a':
          avalue = optarg;
          break;
          case '?':
          if ((optopt == 'p') || (optopt == 'f')) {
              fprintf (stderr, REQUIRED_ARG_$OPT_MSG, optopt);
              print_usage();
              return 1;
          } else if (isprint (optopt)) {
              fprintf (stderr, UNKNOWN_OPT_$OPT_MSG, optopt);
              print_usage();
              return 1;
          } else {
              fprintf (stderr, UNKNOWN_OPT_CHAR_$OPT_MSG, optopt);
              return 1;
          }
          default:
              print_usage();
              return 1;
      }
    }

    if (hflag){
         print_usage();
         return 0;
    }

    if (vflag){
         print_version();
         return 0;
    }

    if (optind < argc){
         print_usage();
         return 1;
    }
           
    pUsername = get_user_name();
    if (pUsername == NULL) {
        fprintf(stderr, "%s\n", GET_USERNAME_ERROR_MSG);
        return 1;
    }
      
    if (avalue == NULL) {
        avalue = DEFAULT_LATCH_ACCOUNTS_FILE;
        aperms = 1;
    } else if (access(avalue, W_OK|R_OK) != 0) {
        fprintf(stderr, ACCESS_RW_ERROR_$USER_$FILE_MSG, pUsername, avalue);
        return 1;
    }
      
    if (fvalue == NULL) {
        fvalue = DEFAULT_LATCH_CONFIG_FILE;
        fperms = 1;
    } else if (access(fvalue, R_OK) != 0) {
        fprintf(stderr, ACCESS_R_ERROR_$USER_$FILE_MSG, pUsername, fvalue);
        return 1;
    }
      
    if (!fperms && drop_privileges(0)) {
        printf("%s\n", DROP_PRIVS_ERROR_MSG);
        return 1;
    }

    pAppId = getConfig(APP_ID_LENGTH, "app_id", fvalue);
    pSecretKey = getConfig(SECRET_KEY_LENGTH, "secret_key", fvalue);

    if(pAppId == NULL || pSecretKey == NULL || strcmp(pAppId,"") == 0 || strcmp(pSecretKey,"") == 0){
        fprintf(stderr, READ_FILE_ERROR_$CFILE_MSG, fvalue);
        return 1;
    }

    if(ovalue && (pOperationId = getConfig(OPERATION_ID_LENGTH, ovalue, fvalue) == NULL)) {
        fprintf(stderr, STATUS_NOT_OP_ERROR_$OP_$CFILE_MSG, ovalue, fvalue);
        return 1;
    }

    pHost = getConfig(MAX_SIZE, "latch_host", fvalue);
    if(pHost == NULL) {
        pHost = malloc(LATCH_API_HOST_LENGTH + 1);
        memset((char*)pHost, 0, LATCH_API_HOST_LENGTH + 1);
        strncpy((char*)pHost, LATCH_API_HOST, LATCH_API_HOST_LENGTH);
    }

    pTimeout = getConfig(TIMEOUT_MAX_LENGTH, "timeout", fvalue);
    if(pTimeout == NULL || ((timeout = atoi(pTimeout)) < TIMEOUT_MIN) || timeout > TIMEOUT_MAX) {
        timeout = 2;
    }
    free((char*)pTimeout);

    if (!aperms && drop_privileges(0)) {
        printf("%s\n", DROP_PRIVS_ERROR_MSG);
        return 1;
    } 

    if (aperms && !fperms && restore_privileges()) {
        printf("%s\n", RESTORE_PRIVS_ERROR_MSG);
    }    

    pAccountId = getAccountId(pUsername, avalue);
 
	if (drop_privileges(0)) {
    	printf("%s\n", DROP_PRIVS_ERROR_MSG);
    	return 1;
	}

    init(pAppId, pSecretKey);
    setHost(pHost);    
    setTimeout(timeout);

    if (sflag) {
        res = latch_status(pUsername, pAccountId);
    } else if (ovalue) {
        res = latch_operation_status(pUsername, pAccountId, pOperationId);
    } else if (uflag) {
        res = latch_unpair(pUsername, pAccountId, avalue, aperms);
    } else if (pvalue) {
        res = latch_pair(pUsername, pAccountId, avalue, aperms, pvalue);
    }

    free((char*)pAccountId);
    free((char*)pAppId); 
    free((char*)pSecretKey);
    free((char*)pHost);
    free((char*)pOperationId);
    return res;
}
