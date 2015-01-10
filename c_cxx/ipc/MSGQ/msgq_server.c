#include<string.h>
#include <fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<linux/limits.h>

char * const key_file = "key_file_1";
const int  key_id = 1;
char * const msg_to_send = "msg to send";

#define MSGQ_U_RD 0400
#define MSGQ_U_WR 0200

typedef struct msg_s{
    long mtype;
    char data[0];
}msg_t;

int main()
{
    key_t msgq_key = -1;
    int   msgq_id;
    int   msglen;
    int   ret;
    int   keyfd;
    struct msqid_ds msgqinfo;
    char  key_path_file[PATH_MAX]="./";
    msg_t *msg;

    msglen = strlen(msg_to_send)+sizeof(msg_t)+1;
    msg =(msg_t*) malloc(msglen);
    msg->mtype = 1;
    memset(msg->data,0,msglen);
    strcpy(msg->data,msg_to_send);
    strcat(key_path_file,key_file);
    msgq_key = ftok(key_path_file,key_id);

    if(-1 == msgq_key)
    {
        perror("create msgq key error as ");
        goto exit;
    }
    printf("open key_file : %s  to write\n",key_path_file);
    keyfd = open(key_path_file,O_RDWR|O_TRUNC|O_CREAT,S_IRWXU);
    if(-1 == keyfd)
    {
        perror("open key file failed as ");
        goto exit;
    }
    if(-1 == write(keyfd,&msgq_key,sizeof(msgq_key)))
    {
        perror("write msgq_key to file error");
        goto exit;
    }
    msgq_id = msgget(msgq_key, IPC_CREAT);
    if(-1 == msgq_id)
    {
        perror("create msgq error as ");
        goto exit;
    }

    if(msgctl(msgq_id,IPC_STAT,&msgqinfo))
    {
        perror("get msgq info error ");
        goto exit;
    }
    msgqinfo.msg_perm.uid = geteuid();
    msgqinfo.msg_perm.gid = getegid();
    msgqinfo.msg_perm.cuid = geteuid();
    msgqinfo.msg_perm.cgid = getegid();
    msgqinfo.msg_perm.mode = MSGQ_U_RD|MSGQ_U_WR; 
//    msgqinfo.msg_qbytes = 81920;

    if(msgctl(msgq_id,IPC_SET,&msgqinfo))
    {
        perror("set msgqinfo error");
        goto exit;
    }
    printf("prepare to send msg and msgq qbytes %ld \n",msgqinfo.msg_qbytes);
    ret = msgsnd(msgq_id,msg,msglen,0);
    if(-1 == ret)
    {
        perror("send msg error as ");
        goto exit;
    }
exit:
    printf("server exit.\n");
    return 0;
}
