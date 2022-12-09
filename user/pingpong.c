#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define MSGSIZE 32


int main(int argc,char *argv[]) {
    if (argc >= 2) {
        fprintf(2,"only one params\n");
        exit(1);
    }
    char inbuf[MSGSIZE];
    int fd[2], pid;
    if (pipe(fd) < 0)
        exit(1);
    
    if ((pid = fork()) > 0) {
        write(fd[1],"ping\n",MSGSIZE);
        wait(0);
        if (read(fd[0],inbuf,MSGSIZE) != MSGSIZE)
            exit(1);
        printf("received %s",inbuf);
    } else {
        if (read(fd[0],inbuf,MSGSIZE) != MSGSIZE)
            exit(1);
        printf("received %s",inbuf);
        write(fd[1],"pong\n",MSGSIZE);
    }
    exit(0);
}