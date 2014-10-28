#include <sys/param.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h> 
#include <sys/stat.h>
#include <pwd.h>

#include "drop_privs.h"
   
   
static int   orig_ngroups = -1;
static gid_t orig_gid = -1;
static uid_t orig_uid = -1;
static gid_t orig_groups[NGROUPS_MAX];
   


int drop_privileges(int permanent) {
    gid_t newgid = getgid(), oldgid = getegid();
    uid_t newuid = getuid(), olduid = geteuid();
   
    if (!permanent && orig_uid == -1 && orig_gid == -1 && orig_ngroups == -1) {
        /* Save information about the privileges that are being dropped so that they
         * can be restored later. Only once.
         */
        orig_gid = oldgid;
        orig_uid = olduid;
        orig_ngroups = getgroups(NGROUPS_MAX, orig_groups);
    }

    /* If root privileges are to be dropped, be sure to pare down the ancillary
    * groups for the process before doing anything else because the setgroups(  )
    * system call requires root privileges.  Drop ancillary groups regardless of
    * whether privileges are being dropped temporarily or permanently.
    */
    if (!olduid) setgroups(1, &newgid);
   
    if (newgid != oldgid) {
#if !defined(linux)
        setegid(newgid);
        if (permanent && setgid(newgid) == -1) return -1;
#else
        if (setregid((permanent ? newgid : -1), newgid) == -1) return -1;
#endif
    }
 
    if (newuid != olduid) {
        if (permanent) {
            setuid(newuid);
        } else {
            seteuid(newuid);
        }
    }
 
    /* verify that the changes were successful */
    if (permanent) {
        if (newgid != oldgid && (setegid(oldgid) != -1 || getegid() != newgid))
            return -1;
        if (newuid != olduid && (seteuid(olduid) != -1 || geteuid() != newuid))
            return -1;
    } else {
        if (newgid != oldgid && getegid() != newgid) return -1;
        if (newuid != olduid && geteuid() != newuid) return -1;
    }

    return 0;
}
   
int restore_privileges(void) {
    if (geteuid() != orig_uid)
        if (seteuid(orig_uid) == -1 || geteuid() != orig_uid) return -1;   
    if (getegid() != orig_gid)
        if (setegid(orig_gid) == -1 || getegid() != orig_gid) return -1;
    if (!orig_uid)
        setgroups(orig_ngroups, orig_groups);

    return 0;
}


char *get_user_name(){

    int bufsize;
    if ((bufsize = sysconf(_SC_GETPW_R_SIZE_MAX)) == -1) {
        bufsize = 1024;
    }

    char *buffer = malloc(bufsize);
    struct passwd pwd, *result = NULL;
    if (getpwuid_r(getuid(), &pwd, buffer, bufsize, &result) != 0 || !result) {
        return NULL;
    }

    return pwd.pw_name;
}

const char *get_effective_user_name(){

    int bufsize;
    if ((bufsize = sysconf(_SC_GETPW_R_SIZE_MAX)) == -1) {
        return NULL;
    }

    char *buffer = malloc(bufsize);
    struct passwd pwd, *result = NULL;
    if (getpwuid_r(geteuid(), &pwd, buffer, bufsize, &result) != 0 || !result) {
        return NULL;
    }

    return pwd.pw_name;
}
