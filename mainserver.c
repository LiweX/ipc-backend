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
#include "tools.h"

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

    sqlite3 *db;

    int shared = shmget(ftok(".", 'S'),sizeof(db),
            (IPC_CREAT | 0660));
    if(shared < 0){
        fprintf(stderr, "memoria compartida. %d: %s \n", errno, strerror( errno ));
        exit(EXIT_FAILURE);
    }

    db = shmat(shared, 0, 0);
    if(db == (void* )-1){
        perror("No se asigno el segmento");
        exit(EXIT_FAILURE);
    }

    int r = sqlite3_open_v2("test.db",&db,SQLITE_OPEN_READWRITE|SQLITE_OPEN_FULLMUTEX,NULL);
    if(r!=SQLITE_OK){
        fprintf(stderr, "Open error: %s\n", sqlite3_errmsg(db));
        exit(EXIT_FAILURE);
    }

    int pid;
    pid = fork();
    if(pid==0){
        printf("Levantando servidor tipo A...\n");
        serverA(port,ipv4address,db);
        exit(EXIT_SUCCESS);   
    }
    pid = fork();
    if(pid==0){
        printf("Levantando servidor tipo B...\n");
        serverB(port,ipv4address,db);
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



