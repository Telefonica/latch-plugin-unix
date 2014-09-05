#!/usr/bin/env bash
# run as root
# import EXPECTED RESPONSES VARS
. ./responses.sh


# force unpair
latch -u > /dev/null 2>&1

echo 'Generate a token with your mobile phone, please.'
read -p 'Enter token: ' TOKEN


echo 'Test latch pair: success'

USER=$(whoami)
if latch -p $TOKEN 2>&1 | grep "$TEST_PAIR_SUCCESS_RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'

  # force unpair
  latch -u > /dev/null 2>&1
  exit
fi


echo 'Test latch pair: pair after success pair'
if latch -p $TOKEN 2>&1 | grep "$TEST_PAIR_AFT_SUCC_PAIRED_RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'
fi


echo 'Test latch status: latch ON'
read -p 'Please, lock your latch in your mobile phone application. Then, press any key'
read
if latch -s 2>&1 | grep "$TEST_STATUS_ON_RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'
fi


echo 'Test latch status: latch OFF'
read -p 'Please, open your latch in your mobile phone application. Then, press any key'
if latch -s 2>&1 | grep "$TEST_STATUS_OFF_RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'
fi


echo 'Test latch operation status: latch ON'
read -p 'Please, lock your test_op latch in your mobile phone application. Then, press any key'
read
if latch -o test_op 2>&1 | grep "$TEST_OP_STATUS_ON_RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'
fi


echo 'Test latch operation status: latch OFF'
read -p 'Please, open your test_op latch in your mobile phone application. Then, press any key'
if latch -o test_op 2>&1 | grep "$TEST_OP_STATUS_OFF_RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'
fi

# force unpair
latch -u > /dev/null 2>&1

