#!/usr/bin/env bash
# run as root
# import EXPECTED RESPONSES VARS
. ./responses.sh


echo 'Test latch unpair: success with other paired users with the same accountId'

sudo mv /etc/latch/latch.accounts /etc/latch/latch_orig.accounts

USER=$(whoami)
sudo echo $USER': xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx' \
> /etc/latch/latch.accounts
sudo echo $USER'2: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx' \
>> /etc/latch/latch.accounts
sudo chmod 600 /etc/latch/latch.accounts

if latch -u 2>&1 | grep "$TEST_UNPAIR_SUCC_WITH_OTHER_SAME_ACC_ID_PAIRED_USER_RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'
fi

sudo mv /etc/latch/latch_orig.accounts /etc/latch/latch.accounts
sudo chmod 600 /etc/latch/latch.accounts
