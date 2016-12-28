#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>

#include "jailwrapper.h"

char* slice_cmd(char* cmd, int begin, int end)
{
    char* result;
    memcpy(result, cmd, end - begin);
    return result;
}

char** split_cmd(char* cmd, char delim)
{
    char** result = (char**)(malloc(sizeof(cmd)));
    int index = 0;
    int begin = 0;
    for(int i = 0; i < sizeof(cmd); i++)
    {
        if(cmd[i] == delim)
        {
            result[index] = slice_cmd(cmd, begin, i);
            begin = i + 1;
            index++;
        }
    }
    return result;
}

pid_t jexec(char* cmd, int jid)
{
    pid_t pid = fork();
    if(pid == -1)
        return pid;
    else if(pid == 0)
    {
        char **cmd_list = split_cmd(cmd, ' ');
        jail_attach(jid);
        execve(cmd_list[0], cmd_list, NULL);
        free(cmd_list);
    }
    else
    {
        printf("created jail\n");
    }
    return pid;
}

void set_path(struct jail *j, char *path)
{
    j->path = path;
}

void set_hostname(struct jail *j, char *host)
{
    j->hostname = host;
}

void set_jailname(struct jail *j, char *name)
{
    j->jailname = name;
}

void set_ip4(struct jail *j, struct in_addr *i_addr) {
    j->ip4s++;
    j->ip4 = i_addr;
}

int jail_init(struct jail *j)
{
    return jail(j);
}

pid_t jail_exec(char* cmd, int jid)
{
    return jexec(cmd, jid);
}

struct JailWrapper* new_jail_wrapper(char* cmd)
{
    struct jail *_jail = (struct jail*) calloc(6, sizeof(jail));
    struct in_addr *i_addr = (struct in_addr*) calloc(1, sizeof(struct in_addr));
    inet_aton("0.0.0.0", i_addr);
    _jail->version = 0;
    _jail->path = "/tmp/";
    _jail->hostname = "spawnd";
    _jail->jailname = "spawnd";
    _jail->ip4s = 1;
    _jail->ip4 = i_addr;
    int jid = jail(_jail);
    pid_t pid = jexec(cmd, jid);
    struct JailWrapper *jail_wrapper = (struct JailWrapper*) calloc(3, sizeof(JailWrapper));
    jail_wrapper->bsd_jail = _jail;
    jail_wrapper->pid = pid;
    jail_wrapper->user = getuid();
    return jail_wrapper;
}

void set_wrapper_pid(struct JailWrapper *jw, pid_t pid)
{
    jw->pid = pid;
}

void set_wrapper_uid(struct JailWrapper *jw, uid_t user)
{
    jw->user = user;
}

void destroy(struct JailWrapper *wrapper)
{
    free(*jail);
}
