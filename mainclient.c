#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <signal.h>
#include "clientA.h"
#include "clientB.h"

#define N_ARGS 3

int main(int argc, char* argv[]){

    signal(SIGSTOP,SIG_IGN);
    
    if(argc!=N_ARGS+1){
        printf("Invalid argument\n");
        exit(EXIT_FAILURE);
    }

    char *client_type= argv[1];
    int port = atoi(argv[3]);
    char *address = argv[2];

    if(strcmp(client_type,"A")==0){
        printf("Creando cliente tipo A...\n");
        clientA(port,address);
    }
    if(strcmp(client_type,"B")==0){
        printf("Creando cliente tipo B...\n");
        clientB(port,address);
    }
    exit(EXIT_SUCCESS);    
}