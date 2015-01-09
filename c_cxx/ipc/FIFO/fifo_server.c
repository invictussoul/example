#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/limits.h>

char * const pipe_name = "pipe";

#define CONTENT_SIZE 1024

int main()
{
    char cwd[PATH_MAX];
    int pipefd; 
    int nread;
    char buf[CONTENT_SIZE];
    getcwd(cwd,PATH_MAX);
    strcat(strcat(cwd,"/"),pipe_name);
    printf("create FIFO as %s\n",cwd);
    if(!mkfifo(cwd,S_IRWXU))
    {
        printf("create success.\n");
        pipefd = open(cwd,O_RDONLY);
        if(pipefd == -1)
        {
            perror("open FIFO error");
            goto exit;
        }
        while(1)
        {
            memset(buf,0,CONTENT_SIZE);
            nread = read(pipefd,buf,CONTENT_SIZE);
            if(nread == -1)
            {
                perror("read msg error ");
                goto exit;
            }
            else
            {
                printf("recv msg %d %s\n",nread,buf);

            }
        }
    }else
    {
        perror("create FIFO failed as :\0");
    }
exit:
    printf("server exit\n");
    return 0;
} 