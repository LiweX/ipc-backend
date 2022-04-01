#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../tools/sqlite3.h"
#include "../tools/tools.h"
   
#define BUFF_SIZE 1024*2
char buff_rx[BUFF_SIZE];
char buff_tx[BUFF_SIZE];

int serverA(int port,char* address,sqlite3* db) {

    int sockfd;

    struct sockaddr_in servaddr, cliaddr;
       
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
       
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
       
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = inet_addr(address);
    servaddr.sin_port = htons((u_int16_t)port);
       
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
   
    while(1){

        unsigned int len = sizeof(cliaddr);
        long int n = recvfrom(sockfd, (char *)buff_rx, BUFF_SIZE, 
            MSG_WAITALL, ( struct sockaddr *) &cliaddr,
            &len);
        buff_rx[n] = '\0';

        memset(buff_tx,0,BUFF_SIZE);
        char *err_msg=0;

        int r = sqlite3_exec(db,buff_rx,callback,0,&err_msg);

        if(r != SQLITE_OK){
            sprintf(buff_tx, "Cannot process query: %s\n", sqlite3_errmsg(db));
            sendto(sockfd, (const char *)buff_tx, strlen(buff_tx), 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
        }
        
        if(r == SQLITE_OK){
            sendto(sockfd, (const char *)buff_tx, strlen(buff_tx), 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
        }
       
    }
    exit(EXIT_SUCCESS);
}

int callback(void *data, int argc, char **argv, 
                    char **azColName) {
    char aux[10000];
    data=data;
    for (int i = 0; i < argc; i++) {

        sprintf(aux,"%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        strcat(buff_tx,aux);
        memset(aux,0,10000);

    }
    return 0;
}