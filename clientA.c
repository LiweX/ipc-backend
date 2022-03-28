#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFF_SIZE 1024*2

// Driver code
int clientA(int port,char *address) {

    int sockfd;
    char *buffer = (char *)malloc(sizeof(char)*BUFF_SIZE);
    char *aux = (char *)malloc(sizeof(char)*BUFF_SIZE);
    char *input = "SELECT * FROM Cars;";

    struct sockaddr_in servaddr;
   
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
   
    memset(&servaddr, 0, sizeof(servaddr));
       
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons((u_int16_t)port);
    servaddr.sin_addr.s_addr = inet_addr(address);

    while(1){

        long int bytes;
        unsigned int len;
        sendto(sockfd, (const char *)input, strlen(input),
            MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
                sizeof(servaddr));

        sprintf(aux,"Query: %s\n",input);

        write(1,aux,strlen(aux));
        memset(aux,0,BUFF_SIZE);
            
        bytes = recvfrom(sockfd, (char *)buffer, BUFF_SIZE, 
                    MSG_WAITALL, (struct sockaddr *) &servaddr,
                    &len);
        buffer[bytes] = '\0';

        sprintf(aux,"Server: %s\n",buffer);

        write(1,aux,strlen(aux));
        memset(aux,0,BUFF_SIZE);
        memset(buffer,0,BUFF_SIZE);
    
        sleep(1);

    }
    
}