#include "config.h"

#define LATCH_USAGE "Usage: latch -p PAIRING_CODE [options]\n"\
                    "       latch -s [options]\n"\
                    "       latch -o OPERATION [options]\n"\
                    "       latch -u [options]\n"\
                    "\n"\
                    "-p PAIRING_CODE,         pair user with Latch servers\n"\
                    "-s,                      check latch status with Latch servers\n"\
                    "-o OPERATION,            check OPERATION status with Latch servers\n"\
                    "-u,                      unpair user from Latch servers\n"\
                    "-v,                      version\n"\
                    "-h,                      help\n"\
                    "\n"\
                    "Options:\n"\
                    "-f CONFIG_FILE,          get configuration parameters from CONFIG_FILE;\n"\
                    "                         by default use " LATCH_CONF_DIR "/latch.conf\n"\
                    "-a ACCOUNTS_FILE,        put and get accounts information from ACCOUNTS_FILE;\n"\
                    "                         by default use " LATCH_CONF_DIR "/latch.accounts\n"\
                    "\n"\
                    "Report latch bugs to " PACKAGE_BUGREPORT "\n"\
                    "For more information, <https://latch.elevenpaths.com/>\n"


#define LATCH_ERROR_102_MSG "Invalid Application ID or Secret Key"
#define LATCH_ERROR_109_MSG "Something went wrong. Review your date & time settings."
#define LATCH_ERROR_201_$USER_MSG "The user %s is not really paired with latch. Try to unpair and pair again.\n"
#define LATCH_ERROR_206_MSG "Invalid token"
#define LATCH_ERROR_301_$USER_MSG "The user %s is not really paired with latch or Invalid Operation ID\n"

#define INVALID_TOKEN_FORMAT_MSG "Invalid token"
#define PAIRING_ERROR_DEFAULT_MSG "There has been an error pairing the user"

#define STATUS_ERROR_DEFAULT_MSG "There has been an error checking status for the user"
#define STATUS_NOT_OP_ERROR_$OP_$CFILE_MSG "Operation %s does not exist in %s\n"
#define DEFAULT_OPT_ERROR_MSG "You have not defined a default option, so we choose 'open'"

#define CONNECTION_SERVER_ERROR_MSG "Error contacting latch servers"

#define LATCH_STATUS_ON_MSG "Status: latch not locked"
#define LATCH_STATUS_OFF_MSG "Status: latch locked"

#define PAIRING_SUCCESS_MSG "Account successfully paired to the user"
#define UNPAIRING_SUCCESS_$USER_MSG "Account belonging to the user %s successfully unpaired from latch\n"

#define ALREADY_PAIRED_$USER_MSG "The user %s is already paired with latch\n"
#define NOT_PAIRED_$USER_MSG "The user %s is not paired with latch\n"

#define CHECK_STATUS_$USER_MSG "Checking status for user %s...\n"
#define CHECK_STATUS_$USER_$OP_MSG "Checking status for user %s in operation...\n"

#define GET_USERNAME_ERROR_MSG "Unknown user"

#define ACCESS_R_ERROR_$USER_$FILE_MSG "The user %s doesn't have the correct permissions to read %s\n"
#define ACCESS_RW_ERROR_$USER_$FILE_MSG "The user %s doesn't have the correct permissions to write or read %s\n"
#define READ_FILE_ERROR_$CFILE_MSG "Failed to read %s\n"
#define WRITE_ACC_FILE_ERROR_MSG "Error writing to the accounts file"

#define UNKNOWN_OPT_CHAR_$OPT_MSG "Unknown option character `\\x%x'.\n"
#define UNKNOWN_OPT_$OPT_MSG "Unknown option `-%c'.\n"
#define REQUIRED_ARG_$OPT_MSG "Option -%c requires an argument.\n"

#define DROP_PRIVS_ERROR_MSG "Couldn't drop privileges."
#define RESTORE_PRIVS_ERROR_MSG "Couldn't restore privileges."
