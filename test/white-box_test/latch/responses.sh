#!/usr/bin/env bash

USER=$(whoami)

TEST_ACCOUNTS_ACCESS_ERROR_RES="The user "$USER" doesn't have the correct permissions to write or read .*"
TEST_CONFIG_ACCESS_ERROR_RES="The user "$USER" doesn't have the correct permissions to read .*"

TEST_PAIR_AFT_SUCC_PAIRED_RES="The user "$USER" is already paired with latch"
TEST_PAIR_BAD_CHAR_TOKEN_RES="Invalid token" 
TEST_PAIR_BAD_LEN_TOKEN_RES="Invalid token"
TEST_PAIR_WITHOUT_NETWORK_RES="Error contacting latch servers"
TEST_PAIR_SUCCESS_RES="Account successfully paired to the user "$USER
TEST_PAIR_INVALID_APP_AND_SEC_RES="Invalid Application ID or Secret Key"
#TEST_PAIR_109_ERR_RES="Something went wrong. Review your date & time settings"
TEST_PAIR_INVALID_TOKEN_RES="Invalid token"
#TEST_PAIR_DEFAULT_ERR_RES="There has been an error pairing the user"

TEST_STATUS_WITHOUT_ACC_FILE_RES="The user "$USER" is not paired with latch"
TEST_STATUS_NOT_PAIRED_RES="The user "$USER" is not paired with latch"
TEST_STATUS_WITHOUT_NETWORK_RES="Error contacting latch servers"
TEST_STATUS_OFF_RES="Status: latch not locked" 
TEST_STATUS_ON_RES="Status: latch locked"
TEST_STATUS_INVALID_APP_AND_SEC_RES="Invalid Application ID or Secret Key"
#TEST_STATUS_109_ERR_RES="Something went wrong. Review your date & time settings"
#TEST_STATUS_201_ERR_RES="The user %s is not really paired with latch. Try to unpair and pair again"
TEST_STATUS_INVALID_ACC_ID_RES="There has been an error checking status for the user"

TEST_OP_STATUS_WITHOUT_ACC_FILE_RES="The user "$USER" is not paired with latch"
TEST_OP_STATUS_NOT_PAIRED_RES="The user "$USER" is not paired with latch"
TEST_OP_STATUS_NOT_OPERATION_RES="Operation .* does not exist in "
TEST_OP_STATUS_WITHOUT_NETWORK_RES="Error contacting latch servers"
TEST_OP_STATUS_OFF_RES="Status: latch not locked" 
TEST_OP_STATUS_ON_RES="Status: latch locked"
TEST_OP_STATUS_INVALID_APP_AND_SEC_RES="Invalid Application ID or Secret Key"
#TEST_OP_STATUS_109_ERR_RES="Something went wrong. Review your date & time settings"
TEST_OP_STATUS_301_ERR_RES="The user "$USER" is not really paired with latch or Invalid Operation ID"
TEST_OP_STATUS_INVALID_ACC_ID_RES="There has been an error checking status for the user"

TEST_UNPAIR_WITHOUT_ACC_FILE_RES="The user "$USER" is not paired with latch"
TEST_UNPAIR_NOT_PAIRED_RES="The user "$USER" is not paired with latch"
TEST_UNPAIR_SUCC_WITH_OTHER_SAME_ACC_ID_PAIRED_USER_RES="Account belonging to the user "$USER" successfully unpaired from latch"
TEST_UNPAIR_SUCCESS_RES="Account belonging to the user "$USER" successfully unpaired from latch"
