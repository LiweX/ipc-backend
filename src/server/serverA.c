#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../tools/tools.h"
   
#define BUFF_SIZE 1024*2
char buff_rxA[BUFF_SIZE];
char buff_txA[BUFF_SIZE];

int callbackA(void *data, int argc, char **argv, 
                    char **azColName);

int serverA(int port,char* address,sqlite3** pool,int *flags) {

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
        long int n = recvfrom(sockfd, (char *)buff_rxA, BUFF_SIZE, 
            MSG_WAITALL, ( struct sockaddr *) &cliaddr,
            &len);
        buff_rxA[n] = '\0';

        bzero(buff_txA,BUFF_SIZE);

        sqlite3 *db;

        int n_db = get_db(flags);
        if(n_db == 5){
            sendto(sockfd, "Empty pool", 10, 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
            continue;
        }else{
            db = pool[n_db];
            flags[n_db]=1;
        }

        char *err_msg=0;

        int r = sqlite3_exec(db,buff_rxA,callbackA,0,&err_msg);

        if(r != SQLITE_OK){
            sprintf(buff_txA, "Cannot process query: %s\n", sqlite3_errmsg(db));
            sendto(sockfd, (const char *)buff_txA, strlen(buff_txA), 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
        }
        
        if(r == SQLITE_OK){
            sendto(sockfd, (const char *)buff_txA, strlen(buff_txA), 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
        }

        flags[n_db]=0;
       
    }
    exit(EXIT_SUCCESS);
}

int callbackA(void *data, int argc, char **argv, 
                    char **azColName) {
    char aux[10000];
    data=data;
    for (int i = 0; i < argc; i++) {

        sprintf(aux,"%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        strcat(buff_txA,aux);
        memset(aux,0,10000);

    }
    return 0;
}