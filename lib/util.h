/*
 * This library offers some util VARS and functions for plugin usage 
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

#include "config.h"

#define OTP_LENGTH 6
#define MAX_SIZE 64
#define APP_ID_LENGTH 20
#define SECRET_KEY_LENGTH 40
#define OPERATION_ID_LENGTH 20
#define ACCOUNT_ID_LENGTH 64
#define DEFAULT_OPTION_MAX_LENGTH 5
#define LATCH_API_HOST_LENGTH 29
#define TIMEOUT_MAX_LENGTH 2
#define TIMEOUT_MIN 1
#define TIMEOUT_MAX 99

#define SEPARATOR "/"

#define LATCH_CONF_FILE "latch.conf"
#define LATCH_ACC_FILE "latch.accounts"
#define DEFAULT_LATCH_CONFIG_FILE LATCH_CONF_DIR SEPARATOR LATCH_CONF_FILE
#define DEFAULT_LATCH_ACCOUNTS_FILE LATCH_CONF_DIR SEPARATOR LATCH_ACC_FILE

#define LATCH_API_HOST "https://latch.elevenpaths.com"
#define LATCH_TEMP_FILE "/tmp/latch_temp"

#define LATCH_CONF_PERMS 0600


int countAccountId(const char* pAccountId, const char* pAccounts);
int appendAccountId(const char* pUsername, const char* acc_id, const char* pAccounts);
int deleteAccountId(const char* pUsername, const char* pAccounts);

const char* getAccountId(const char* pUser, const char* pAccounts);
const char* getConfig(int max_size, const char* pParameter, const char* pConfig);

void send_syslog_alert(char *ident, const char *msg);
