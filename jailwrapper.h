#include <unistd.h>
#include <sys/param.h>
#include <sys/jail.h>
#include <sys/types.h>
#include <arpa/inet.h>

#ifndef IN_C
#define IN_C true
#endif

#ifndef JAILWRAPPER_H_
#define JAILWRAPPER_H_
    typedef struct JailWrapper
    {
        struct jail *bsd_jail;
        uid_t user;
        pid_t pid;
    } JailWrapper;

    void set_path(struct jail *j, char *path);

    void set_hostname(struct jail *j, char *host);

    void set_jailname(struct jail *j, char *name);

    void set_ip4(struct jail *j, struct in_addr *i_addr);

    int jail_init(struct jail *j);

    pid_t jail_exec(char* cmd, int jid);

    struct JailWrapper* new_jail_wrapper(char* cmd);

    void set_wrapper_pid(struct JailWrapper *jw, pid_t pid);

    void set_wrapper_uid(struct JailWrapper *jw, uid_t user);

    void destroy(struct JailWrapper *wrapper);
#endif
