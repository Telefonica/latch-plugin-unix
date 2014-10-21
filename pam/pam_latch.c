/*
 * This library integrates Latch with PAM modules in some UNIX systems (like Linux).
 * that are continuously exposed in the Internet, like ssh
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


// standard stuff
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <syslog.h>

#include <config.h>

// pam stuff
#ifdef HAVE_SECURITY_PAM_MODULES_H
#include <security/pam_modules.h>
#endif

#ifdef HAVE_SECURITY_PAM_APPL_H
#include <security/pam_appl.h>
#endif

#include "../lib/latch.h"
#include "../lib/util.h"
 #include "../lib/drop_privs.h"




/* expected hook */
PAM_EXTERN int pam_sm_setcred( pam_handle_t *pamh, int flags, int argc, const char **argv ) {
    return PAM_SUCCESS;
}

/*
 * Makes getting arguments easier. Accepted arguments are of the form: name=value
 * 
 * @param pName- name of the argument to get
 * @param argc- number of total arguments
 * @param argv- arguments
 * @return Pointer to value or NULL
 */
static const char* getArg(const char* pName, int argc, const char** argv) {
    int len = strlen(pName);
    int i;

    for (i = 0; i < argc; i++) {
        if (strncmp(pName, argv[i], len) == 0 && argv[i][len] == '=') {
            // only give the part url part (after the equals sign)
            return argv[i] + len + 1;
        }
    }
    return 0;
}

char *get_response(pam_handle_t *pamh, const char *prompt, int verbose) {
    struct pam_conv *conv;
    int retval;
    struct pam_message msg;
    const struct pam_message *msgp;
    struct pam_response *resp;
    char *response;
    char buffer[512];

    retval = pam_get_item(pamh, PAM_CONV, (const void**) &conv);
    if (retval != PAM_SUCCESS) {
        return NULL;
    }

    /* check if we want verbose input */
    if ( verbose != 0 )
        msg.msg_style = PAM_PROMPT_ECHO_ON;
    else
        msg.msg_style = PAM_PROMPT_ECHO_OFF;

    if (prompt) {
        sprintf(buffer, "%s: ", prompt);
    } else {
        strcpy(buffer, "Password: ");
    }

    /* set up the conversation */
    msg.msg = buffer;
    msgp = &msg;
    resp = NULL;
    response = NULL;
    retval = (*conv->conv)(1, &msgp, &resp, conv->appdata_ptr);
    if (resp != NULL) {
        if (retval == PAM_SUCCESS) {
            response = resp->resp;
        } else {
            free(resp->resp);
        }
        free(resp);
    }

    return response;
}


/* expected hook, this is where custom stuff happens */
PAM_EXTERN int pam_sm_authenticate(pam_handle_t* pamh, int flags, int argc, const char **argv) {
    int ret = 0;

    const char *pUsername = NULL;                
    const char *pAccountId = NULL;
    const char *pOperation = NULL;
    const char *pSecretKey = NULL;
    const char *pAppId = NULL;
    const char *pOperationId = NULL;
    const char *pAccounts = NULL;
    const char *pConfig = NULL;
    const char *pOtp = NULL;
    const char *pHost = NULL;
    const char *pTimeout = NULL;
    char *pDefaultOption = NULL;
    char *buffer;       
    int timeout = 2; 
    int default_option = PAM_SUCCESS;  
    int res =  PAM_SUCCESS;   
    char *otp = NULL;
    
    
    struct pam_message msg[1],*pmsg[1];
    struct pam_response *resp;

    // setting up conversation call prompting for one-time code 
    pmsg[0] = &msg[0] ;
    msg[0].msg_style = PAM_PROMPT_ECHO_ON ;
    msg[0].msg = "One-time code: " ;
    resp = NULL ;
    

    if (pam_get_user(pamh, &pUsername, NULL) != PAM_SUCCESS) {
        return PAM_AUTH_ERR;
    }

    pAccounts = getArg("accounts", argc, argv);
    if (!pAccounts) {
        pAccounts = DEFAULT_LATCH_ACCOUNTS_FILE;
    }

    pAccountId = getAccountId(pUsername, pAccounts);
    if (pAccountId == NULL) {
        return PAM_SUCCESS; 
    }

    pConfig = getArg("config", argc, argv);
    if (!pConfig) {
        pConfig = DEFAULT_LATCH_CONFIG_FILE;
    }
    
    pOperation = getArg("operation", argc, argv);
    if (!pOperation) {
        pOperationId = NULL;
    } else {
        pOperationId = getConfig(OPERATION_ID_LENGTH, pOperation, pConfig);
        if (pOperationId == NULL) {
            send_syslog_alert("PAM", "Latch-auth-pam error: Failed to find operation");
            perror("Failed to find operation");
            return PAM_SUCCESS;
        } 
    }

    pOtp = getArg("otp", argc, argv);
    if (!pOtp) {
        pOtp = "no";
    }
    
    pDefaultOption = (char *)getConfig(DEFAULT_OPTION_MAX_LENGTH, "action", pConfig);
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
        default_option = PAM_SUCCESS;
    } else {
        default_option = PAM_AUTH_ERR;
    }
    free(pDefaultOption);
   
    pAppId = getConfig(APP_ID_LENGTH, "app_id", pConfig);
    pSecretKey = getConfig(SECRET_KEY_LENGTH, "secret_key", pConfig);

    if(pAppId == NULL || pSecretKey == NULL || strcmp(pAppId, "") == 0 || strcmp(pSecretKey, "") == 0){
        send_syslog_alert("PAM", "Latch-auth-pam error: Failed to read \"latch.conf\"");
        perror("Failed to read \"latch.conf\"");
        free((char*)pAccountId);
        free((char*)pOperationId);
        return PAM_SUCCESS;
    }

    pHost = getConfig(MAX_SIZE, "latch_host", pConfig);
    if(pHost == NULL) {
        pHost = malloc(LATCH_API_HOST_LENGTH + 1);
        memset((char*)pHost, 0, LATCH_API_HOST_LENGTH + 1);
        strncpy((char*)pHost, LATCH_API_HOST, LATCH_API_HOST_LENGTH);
    }

    pTimeout = getConfig(TIMEOUT_MAX_LENGTH, "timeout", pConfig);
    if(pTimeout == NULL || ((timeout = atoi(pTimeout)) < TIMEOUT_MIN) || timeout > TIMEOUT_MAX) {
        timeout = 2;
    }
    free((char*)pTimeout); 
 
    if (drop_privileges(0)) {
        send_syslog_alert("PAM", "Latch-auth-pam error: Couldn't drop privileges");
    }

    init(pAppId, pSecretKey);
    setHost(pHost);
    setTimeout(timeout);

    if (pOperationId != NULL) {
        buffer = operationStatus(pAccountId, pOperationId);
    } else {
        buffer = status(pAccountId);
    }
    free((char*)pAppId); 
    free((char*)pSecretKey);
    free((char*)pAccountId);
    free((char*)pOperationId);
    free((char*)pHost);
 
    if (restore_privileges()) {
        send_syslog_alert("PAM", "Latch-auth-pam error: Couldn't restore privileges");
    }

    if(buffer == NULL || strcmp(buffer,"") == 0){
        free(buffer);
        return default_option;
    }

    if (strstr(buffer, "\"status\":\"off\"") != NULL){
        fprintf (stderr, "AUTH-PAM: latch locked\n");
        send_syslog_alert("PAM", "Latch-auth-pam warning: Someone tried to access. Latch locked");
        res = PAM_AUTH_ERR;

    }else if (strstr(buffer, "\"status\":\"on\"") != NULL) {
        
        if(strncmp(pOtp, "yes", 3) == 0){

            char *pch;
            if((pch = strstr(buffer, "\"two_factor\"")) != NULL){
                char code[OTP_LENGTH];
                memset(code, 0, OTP_LENGTH);

                strncpy (code, pch + strlen("\"two_factor\":{\"token\":\""), OTP_LENGTH);
                
                otp = get_response(pamh, "One-time password", 1);
     
                // comparing user input with known code 
                if(strncmp(code, otp, OTP_LENGTH) != 0  || strlen(otp) != OTP_LENGTH){
                    send_syslog_alert("PAM", "Latch-auth-pam warning: Someone tried to access. Bad OTP");
                    res = PAM_AUTH_ERR;
                } else {
                    res = PAM_SUCCESS;
                }                    
            }
            
        }else{
            res = PAM_SUCCESS;
        }
    }

    free(buffer);
    return res;
}
