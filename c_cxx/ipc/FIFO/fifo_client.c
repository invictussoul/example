#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/limits.h>

char * const pipe_name = "pipe";

int main()
{
    char cwd[PATH_MAX];
    int pipefd; 
    int nwrite;
    char buf[]="msg to send";
    getcwd(cwd,PATH_MAX);
    strcat(strcat(cwd,"/"),pipe_name);
    printf("open FIFO as %s\n",cwd);
    pipefd = open(cwd,O_WRONLY);
    if(pipefd == -1)
    {
        perror("open FIFO error\n");
        return ;
    }
    while(1)
    {
        nwrite = write(pipefd,buf,sizeof(buf));
        if(nwrite !=sizeof(buf))
        {
            perror("write msg error ");
            goto exit;
        }
        else
        {
            printf("write msg  %s success .\n",buf);
        }
    }
exit:
    printf("client exit\n");
    return 0;
} 