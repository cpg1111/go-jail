#include <net/inet.h>
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
    char* result[sizeof(cmd)];
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
        char **cmd_list;
        jail_attach(jid);
        execve(cmd_list[0], cmd_list, NULL);
    }
    else
    {
        printf("created jail\n");
    }
}

struct JailWrapper* new_jail_wrapper(char* cmd)
{
    struct jail *_jail = (struct jail*) calloc(6, sizeof(jail));
    struct in_addr *i_addr = (struct in_addr*) calloc(1, sizeof(in_addr));
    inet_aton("0.0.0.0", i_addr);
    _jail->version = "10.2";
    _jail->path = "/tmp/";
    _jail->hostname = "spawnd";
    _jail->jailname = "spawnd";
    _jail->ip4s = 1;
    _jail->ip4 = i_addr;
    int jid = jail(_jail);
    pid_t pid = jexec(cmd, jid);
    struct JailWrapper *jail_wrapper = (struct JailWrapper*) calloc(3, JailWrapper);
    jail_wrapper->bsd_jail = _jail;
    jail_wrapper->pid = pid;
    jail_wrapper->user = getuid();
}

void destroy(struct JailWrapper *wrapper)
{
    free(*jail);
}