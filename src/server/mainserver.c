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
#include <sys/mman.h>
#include "serverA.h"
#include "serverB.h"
#include "serverC.h"
#include "../tools/tools.h"

#define N_ARGS 5

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
    char dbname[100];
    bzero(dbname,100);
    strcpy(dbname,argv[5]);
    strcat(dbname,".db");

    sqlite3** pool = (sqlite3 **)mmap(NULL, sizeof(sqlite3*)*POOL_SIZE, 
                        PROT_READ | PROT_WRITE, 
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if(pool == MAP_FAILED){
        perror("No se asigno el segmento");
        exit(EXIT_FAILURE);
    }

    int* flags = (int*)mmap(NULL, sizeof(int)*POOL_SIZE, 
                    PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if(flags == MAP_FAILED){
        perror("No se asigno el segmento");
        exit(EXIT_FAILURE);
    }

    prepare_pool(pool,dbname,flags);


    int pid;
    pid = fork();
    if(pid==0){
        printf("Levantando servidor tipo A...\n");
        serverA(port,ipv4address,pool,flags);
        exit(EXIT_SUCCESS);   
    }
    pid = fork();
    if(pid==0){
        printf("Levantando servidor tipo B...\n");
        serverB(port,ipv4address,pool,flags);
        exit(EXIT_SUCCESS);   
    }
    pid = fork();
    if(pid==0){
        printf("Levantando servidor tipo C...\n");
        serverC(port,ipv6address,interface,pool,flags,dbname);
        exit(EXIT_SUCCESS);   
    }
    while(1){}
}







