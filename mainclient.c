#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <signal.h>
#include "clientA.h"
#include "clientB.h"
#include "clientC.h"

#define N_ARGS 4

int main(int argc, char* argv[]){

    signal(SIGSTOP,SIG_IGN);
    
    if(argc!=N_ARGS+1){
        printf("Invalid argument\n");
        exit(EXIT_FAILURE);
    }

    char *client_type= argv[1];
    int port = atoi(argv[3]);
    char *address = argv[2];
    char *interface = argv[4];

    if(strcmp(client_type,"A")==0){
        printf("Creando cliente tipo A...\n");
        clientA(port,address);
    }
    if(strcmp(client_type,"B")==0){
        printf("Creando cliente tipo B...\n");
        clientB(port,address);
    }
    if(strcmp(client_type,"C")==0){
        printf("Creando cliente tipo C...\n");
        clientC(port,address,interface);
    }
    exit(EXIT_SUCCESS);    
}