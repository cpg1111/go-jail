#include <unistd.h>
#include <sys/param.h>
#include <sys/jail.h>
#include <sys/types.h>

#ifndef JAILWRAPPER_H_
#define JAILWRAPPER_H_
    typedef struct 
    {
        jail *bsd_jail;
        uid_t user;
        pid_t pid;
    } JailWrapper;

    struct JailWrapper* new_jail_wrapper(char* cmd);

    void destroy(struct JailWrapper *wrapper);
#endif
