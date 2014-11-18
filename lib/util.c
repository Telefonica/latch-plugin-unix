/*
 * This library implements some util functions for plugin usage.
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
#include <stdlib.h>
#include <string.h>
#include <syslog.h> 
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include "util.h"



int countAccountId(const char* pAccountId, const char* pAccounts) {

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *fp;
    int i = 0;

    if (pAccountId == NULL) {
        return -1;
    }

    if (strlen(pAccountId) != ACCOUNT_ID_LENGTH) {
        return -1;
    }

    fp = fopen(pAccounts,"r");
    if (fp == NULL) {
        return -1;
    }

    while((read = getline(&line,&len, fp)) != -1){
        if(strstr(line, pAccountId) != NULL){
            i++;
        }
    }

    free(line);
    fclose(fp);
    return i;
}

int appendAccountId(const char* pUsername, const char* acc_id, const char* pAccounts) {

    FILE *fp;

    fp = fopen(pAccounts,"a+");
    if (fp == NULL) {
        return -1;
    }

    fprintf(fp, "%s: %s\n", pUsername, acc_id);
    fclose(fp);

    return 0;
}

int deleteAccountId(const char* pUsername, const char* pAccounts) {

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *fp;
    int fp_dest;
    char nameBuff[32];

    fp = fopen(pAccounts, "r");
    if (fp == NULL) {
        return -1;
    }

    strncpy(nameBuff,"/tmp/latch-XXXXXX",17);
    nameBuff[17] = '\0';

    fp_dest = mkstemp(nameBuff);
    if (fp_dest == -1) {
        fclose(fp);
        return -1;
    }
   
    while ((read = getline(&line,&len, fp)) != -1) {
        if (! (strncmp(pUsername, line, strlen(pUsername)) == 0  &&  line[strlen(pUsername)] == ':')) {
            if (write(fp_dest, line, read) != read) {
                free(line);
                close(fp_dest);
                fclose(fp); 
                return -1;  
            }
        }
    }

    free(line);
    close(fp_dest);
    fclose(fp); 

    if (rename(nameBuff, pAccounts) != 0) {
        return 1;
    }

    if (chmod (pAccounts, LATCH_CONF_PERMS) < 0) {
        return 1;
    }

    return 0;
}

const char* getAccountId(const char* pUser, const char* pAccounts) {

    char *toReturn = NULL;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *fp;

    fp = fopen(pAccounts,"r");
    if (fp == NULL) {
        return NULL;
    }

    while((read = getline(&line,&len, fp)) != -1){
        if((read == strlen(pUser) + ACCOUNT_ID_LENGTH + 3)  &&  
                strncmp(pUser, line, strlen(pUser)) == 0  &&  
                line[strlen(pUser)] == ':'){
            toReturn = malloc(ACCOUNT_ID_LENGTH + 1);
            strncpy(toReturn, line + strlen(pUser) + 2, ACCOUNT_ID_LENGTH);
            toReturn[ACCOUNT_ID_LENGTH] = '\0';
            break;
        }
    }

    free(line);
    fclose(fp);
    return toReturn;
}

const char *getConfig(int max_size, const char* pParameter, const char* pConfig) {

    char *toReturn = NULL;
    char buffer[BUFSIZ], *ptr;

    FILE *fp = fopen(pConfig,"r");
    if (fp == NULL) {
        return NULL;
    }

    while(fgets(buffer, BUFSIZ, fp) != NULL) {
        ptr = buffer;
        while((*ptr == ' ') || (*ptr == '\t'))
            ptr++;
        if ((*ptr == '\n') || (*ptr == '#')) /* Move to the next line */
            continue;
        if (strncmp(ptr, pParameter, strlen(pParameter)) == 0) {
            ptr += strlen(pParameter);
            while((*ptr == ' ') || (*ptr == '\t'))
                ptr++;
            if (*ptr != '=') { /* Move to the next line */
                continue;
            }
            ptr++;
            while ((*ptr == ' ') || (*ptr == '\t')) {
                ptr++;
            }
            if (*ptr != '\n') {
                toReturn = malloc(max_size + 1);
                memset(toReturn, 0, max_size + 1);
                strncpy(toReturn, (char *)ptr, max_size);
                if (toReturn[strlen(toReturn)-1] == '\n') {
                    toReturn[strlen(toReturn)-1] = '\0';
                }
                break;
            }
        }
    }

    fclose(fp);
    return toReturn;
}


/*
 * Log a message into auth logger
 * 
 * @param Message
 */
void send_syslog_alert(char *ident, const char *msg){  
    openlog (ident, LOG_PID, LOG_AUTH);    
    syslog (LOG_ALERT, msg); 
    closelog ();
}
