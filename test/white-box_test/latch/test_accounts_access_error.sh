#!/usr/bin/env bash
# import EXPECTED RESPONSES VARS
. ./responses.sh


# force unpair
latch -u > /dev/null 2>&1

echo 'Test latch accounts: without read-write permissions'
if latch -a /etc/latch/latch.accounts 2>&1 | grep "$TEST_ACCOUNTS_ACCESS_ERROR_RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'
fi
