#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# vim: set fileencoding=utf-8
# Run as root

'''
 This script allows to pair our application with Latch in some UNIX systems (like Linux)
 Copyright (C) 2013 Eleven Paths

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
'''

import sys
import os
import latch



## const

USER_DEFAULT = 'testlatch'
ACCOUNT_EMAIL_DEFAULT = 'ivan.martin@11paths.com'

LATCH_CONFIG_DEFAULT = '/etc/latch/latch.conf'
LATCH_ACCOUNTS_DEFAULT = '/etc/latch/latch.accounts'



## functions

def get_config(name, configFile=LATCH_CONFIG_DEFAULT):

    if os.path.isfile(configFile):
        f = open(configFile,"r")
        lines = f.readlines()
        f.close()

        for line in lines:
            words = line.split()
            if len(words) == 3 and words[0] == name and words[1] == '=':
                return words[2]   
        return None

    else:
        print (configFile + " doesn't exist")
        exit(1)

def is_paired(user, accountsFile=LATCH_ACCOUNTS_DEFAULT):
    if os.path.isfile(accountsFile):
        f = open(accountsFile,"r")
        lines = f.readlines()
        f.close()

        for line in lines:
            if line.startswith(user + ':'):
                return True
        return False
    else:
        print (accountsFile + " doesn't exist")
        exit(1)

def add_account(user, accountId, accountsFile=LATCH_ACCOUNTS_DEFAULT):
    if os.path.isfile(accountsFile):
        f = open (accountsFile, "a")
        f.write(user + ": " + accountId + "\n")
        f.close();
    else:
        print (accountsFile + " doesn't exist")
        exit(1)



## main ##

if len(sys.argv) == 4:
    user = sys.argv[2]
    accountEmail = sys.argv[3]
else:
    user = USER_DEFAULT
    accountEmail = ACCOUNT_EMAIL_DEFAULT

if is_paired (user):
    print (user + ' is already paired')
    exit(0)

secret_key = get_config("secret_key");
app_id = get_config("app_id");
latch_host = get_config("latch_host")

api = latch.Latch(app_id, secret_key)
if latch_host != None:
    latch.Latch.set_host(latch_host)

res = api.pairWithId(accountEmail)

responseData = res.get_data()
responseError = res.get_error()

if 'accountId' in responseData:
    accountId = responseData["accountId"]
    add_account(user, accountId)
    print (user + ' paired successfully')
    exit(0)
else:
    print ('Error pairing account: ' + str(responseError))
    exit(1)
