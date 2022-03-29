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
#include <signal.h>
#include "serverA.h"
#include "serverB.h"
#include "serverC.h"

#define N_ARGS 4

int main(int argc, char* argv[]){

    signal(SIGSTOP,SIG_IGN);

    if(argc!=N_ARGS+1){
        printf("Invalid arguments\n");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[3]);
    char *ipv4address = argv[1];
    char *ipv6address = argv[2];
    char *interface = argv[4];

    int pid;
    pid = fork();
    if(pid==0){
        printf("Levantando servidor tipo A...\n");
        serverA(port,ipv4address);
        exit(EXIT_SUCCESS);   
    }
    pid = fork();
    if(pid==0){
        printf("Levantando servidor tipo B...\n");
        serverB(port,ipv4address);
        exit(EXIT_SUCCESS);   
    }
    pid = fork();
    if(pid==0){
        printf("Levantando servidor tipo C...\n");
        serverC(port,ipv6address,interface);
        exit(EXIT_SUCCESS);   
    }
    while(1){

    }
}
