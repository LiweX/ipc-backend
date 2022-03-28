#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include "serverA.h"

#define N_ARGS 2

int main(int argc, char* argv[]){

    signal(SIGSTOP,SIG_IGN);

    if(argc!=N_ARGS+1){
        printf("Invalid arguments\n");
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[2]);
    char *ipv4address = argv[1];

    int pid;
    pid = fork();
    if(pid==0){
        printf("Levantando servidor tipo A...\n");
        serverA(port,ipv4address);
        exit(EXIT_SUCCESS);   
    }
    while(1){

    }
}
