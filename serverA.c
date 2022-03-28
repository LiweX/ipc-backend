#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
   
#define BUFF_SIZE 1024
   
// Driver code
int serverA(int port,char* address) {

    int sockfd;
    char buffer[BUFF_SIZE];
    char *aux = (char *)malloc(sizeof(char));
    char *result = (char *)malloc(sizeof(char));

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

        unsigned int len = sizeof(cliaddr);  //len is value/resuslt
        long int n = recvfrom(sockfd, (char *)buffer, BUFF_SIZE, 
            MSG_WAITALL, ( struct sockaddr *) &cliaddr,
            &len);
        buffer[n] = '\0';
        
        sprintf(aux,"Client : %s\n", buffer);
        write(0,aux,strlen(aux));

        sprintf(aux,"Result: %s",buffer);
        strcpy(result,aux);

        sendto(sockfd, (const char *)result, strlen(result), 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
    }
    exit(EXIT_SUCCESS);
}