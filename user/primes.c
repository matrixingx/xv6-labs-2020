#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MSGSIZE 36
#define IsPrime 'T'
#define NotPrime 'F'

void checkPrime(int fdOutput,int fdInput) {
    char primes[MSGSIZE];
    int idx = -1;
    read(fdOutput,primes,MSGSIZE);
    for (int i = 0 ; i < MSGSIZE ; i++) {
        if (primes[i] == IsPrime) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        return;
    }
    printf("prime %d\n",idx);
    primes[idx] = NotPrime;
    for (int i = 0 ; i < MSGSIZE ; i++) {
        if (i % idx == 0) {
            primes[i] = NotPrime;
        }
    }
    if (fork()>0) {
        write(fdInput,primes,MSGSIZE);
    } else {
        checkPrime(fdOutput,fdInput);
    }
}

int main(int argc,char *argv[]) {
    if (argc >= 2) {
        fprintf(2,"only one params\n");
        exit(1);
    }
    int fd[2];
    char primes[MSGSIZE];
    int i;
    if (pipe(fd) < 0) {
        exit(-1);
    }
    for (i = 0 ; i < MSGSIZE ; i++) {
        primes[i] = IsPrime;
    }
    primes[0] = NotPrime;
    primes[1] = NotPrime;
    int pid = fork();
    if (pid>0) {
        write(fd[1],primes,MSGSIZE);
        wait(0);
    } else {
        checkPrime(fd[0],fd[1]);
    }
    wait(0);
    exit(0);
}
