#!/usr/bin/env bash
# run as root
# import EXPECTED RESPONSES VARS
. ./responses.sh


# force unpair
latch -u > /dev/null 2>&1

sudo mv /etc/latch/latch.accounts /etc/latch/latch_orig.accounts

USER=$(whoami)
sudo echo $USER': xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx' \
> /etc/latch/latch.accounts
sudo chmod 600 /etc/latch/latch.accounts

sudo mv /etc/latch/latch.conf /etc/latch/latch_orig.conf
sudo sed 's/app_id.*/app_id = XXXX/g' /etc/latch/latch_orig.conf | \
sudo sed 's/secret_key.*/secret_key = XXXX/g' > /etc/latch/latch.conf
sudo chmod 600 /etc/latch/latch.conf

echo 'Test latch operation status: invalid app_id and secret_key configuration'
if latch -o test_op 2>&1 | grep "$TEST_OP_STATUS_INVALID_APP_AND_SEC_RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'
fi

sudo mv /etc/latch/latch_orig.conf /etc/latch/latch.conf
sudo chmod 600 /etc/latch/latch.conf

sudo mv /etc/latch/latch_orig.accounts /etc/latch/latch.accounts
sudo chmod 600 /etc/latch/latch.accounts
