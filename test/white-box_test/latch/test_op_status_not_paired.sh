#!/usr/bin/env bash
# import EXPECTED RESPONSES VARS
. ./responses.sh


# force unpair
latch -u > /dev/null 2>&1

USER=$(whoami)
echo 'Test latch operation status: user not paired'
if latch -o test_op  2>&1 | grep "$TEST_OP_STATUS_NOT_PAIRED_RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'
fi
