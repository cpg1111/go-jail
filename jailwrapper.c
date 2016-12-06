#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <arpa/inet.h>

#include "jailwrapper.h"

char* slice_cmd(char* cmd, int begin, int end)
{
    char* result;
    memcpy(result, cmd, end - begin);
    return result;
}

void split_cmd(char* cmd, char delim, char** result)
{
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
}

pid_t jexec(char* cmd, int jid)
{
    pid_t pid = fork();
    if(pid == -1)
    {
        return pid;
    }
    else if(pid == 0)
    {
        printf("child\n");
        char *cmd_list[sizeof(cmd)];
        split_cmd(cmd, ' ', (char**)(&cmd_list));
        jail_attach(jid);
        execve(cmd_list[0], cmd_list, NULL);
    }
    else
    {
        printf("created jail\n");
    }
    return pid;
}

char* create_name()
{
    char* result = "test";
    char* alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    // for(int i = 0; i < 16; i++)
    // {
        //time_t clock;
        //result[i] = alphabet[i*sizeof(ctime(&clock))];
    // }
    return result;
}

char* create_tmp_dir(char* name)
{
    char* path;
    sprintf(path, "/tmp/%s/", name);
    int success = mkdir(path, 0755);
    if(success >= 0){
        return path;
    }
    return "";
}

struct JailWrapper* new_jail_wrapper(char* cmd)
{
    struct jail *_jail = (struct jail*) calloc(6, sizeof(jail));
    struct in_addr *i_addr = (struct in_addr*) calloc(1, sizeof(struct in_addr));
    inet_aton("10.0.0.20", i_addr);
    _jail->version = 10;
    _jail->hostname = create_name();
    _jail->jailname = create_name();
    _jail->path = create_tmp_dir(_jail->jailname);
    _jail->ip4s = 1;
    _jail->ip4 = i_addr;
    int jid = jail(_jail);
    printf("created %d\n", jid);
    pid_t pid = jexec(cmd, jid);
    struct JailWrapper *jail_wrapper = (struct JailWrapper*) calloc(3, sizeof(struct JailWrapper));
    jail_wrapper->bsd_jail = _jail;
    jail_wrapper->pid = pid;
    jail_wrapper->user = getuid();
    printf("here\n");
    return jail_wrapper;
}

void destroy(struct JailWrapper *wrapper)
{
    free(wrapper);
}
