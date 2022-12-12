#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#define MSGSIZE 16

int main(int argc,char *argv[]) {
    // 读取之前命令的执行结果作为标准输入
    char buf[MSGSIZE];
    read(0,buf,MSGSIZE);

    // 读取这次的命令
    char *xargv[MAXARG];
    int xargc = 0;
    for (int i = 1 ; i < argc ; i++) {
        xargv[xargc++] = argv[i];
    }

    char *p = buf;
    for (int i = 0 ; i < MSGSIZE;i++) {
        // 找到之前的执行结果中的换行符（至少有一个）
        if (buf[i] == '\n') {
            int pid = fork();
            if (pid > 0) {
                p = &buf[i+1];
                wait(0);
            } else {
                // 在子进程中拼接最终要exec的命令
                buf[i] = 0;
                xargv[xargc++] = p;
                xargv[xargc++] = 0;
                exec(xargv[0],xargv);
                exit(0);
            }
        }
    }
    wait(0);
    exit(0);
}