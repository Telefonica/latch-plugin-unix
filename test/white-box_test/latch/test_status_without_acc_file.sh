#!/usr/bin/env bash
# run as root
# import EXPECTED RESPONSES VARS
. ./responses.sh


# force unpair
latch -u > /dev/null 2>&1

sudo mv /etc/latch/latch.accounts /etc/latch/latch_orig.accounts

echo 'Test latch status: without account file'
if latch -s 2>&1 | grep "$TEST_STATUS_WITHOUT_ACC_FILE_RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'
fi

sudo mv /etc/latch/latch_orig.accounts /etc/latch/latch.accounts
sudo chmod 600 /etc/latch/latch.accounts
