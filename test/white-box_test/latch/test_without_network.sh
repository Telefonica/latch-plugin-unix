#!/usr/bin/env bash
# run as root
# read EXPECTED RESPONSES VARS
. ./responses.sh


# force unpair
latch -u > /dev/null 2>&1


echo 'getting down network...'
sudo ifconfig eth0 down
sleep 2


echo 'Test latch pair: without network'

if latch -p XXXXXX 2>&1 | grep "$TEST_PAIR_WITHOUT_NETWORK_RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'
fi


echo 'Test latch status: without network'

sudo mv /etc/latch/latch.accounts /etc/latch/latch_orig.accounts

USER=$(whoami)
sudo echo $USER': xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx' \
> /etc/latch/latch.accounts
sudo chmod 600 /etc/latch/latch.accounts

if latch -s 2>&1 | grep "$TEST_STATUS_WITHOUT_NETWORK_RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'
fi


echo 'Test latch operation status: without network'

if latch -o test_op   2>&1 | grep "$TEST_OP_STATUS_WITHOUT_NETWORK_RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'
fi

sudo mv /etc/latch/latch_orig.accounts /etc/latch/latch.accounts
sudo chmod 600 /etc/latch/latch.accounts



echo 'geting up network...'

# ubuntu
sudo service network-manager restart > /dev/null 2>&1
sudo ifconfig eth0 up > /dev/null 2>&1

# centos
sudo service network restart > /dev/null 2>&1
sudo ifup eth0 > /dev/null 2>&1

sleep 20
