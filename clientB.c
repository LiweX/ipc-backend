#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sqlite3.h"

#define BUFF_SIZE 1024*2

int clientB(int port, char* address) 
{ 
    int sockfd; 
    struct sockaddr_in servaddr;
    char buff_tx[BUFF_SIZE];
    char buff_rx[BUFF_SIZE];

    /* Socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //tpc socket
    if (sockfd == -1) 
    { 
        printf("CLIENT: socket creation failed...\n"); 
        return -1;  
    } 
    else
    {
        printf("CLIENT: Socket successfully created..\n"); 
    }
    
    memset(&servaddr, 0, sizeof(servaddr));

    /* assign IP, PORT */
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr( address ); 
    servaddr.sin_port = htons((uint16_t)port); 
  
    /* try to connect the client socket to server socket */
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) 
    { 
        printf("connection with the server failed...\n");  
        return -1;
    } 
    memset(buff_rx,0,BUFF_SIZE);
    recv(sockfd,buff_rx,BUFF_SIZE,0);
    write(1,buff_rx,strlen(buff_rx));
  
    /* send test sequences*/
    while(1){

        memset(buff_tx,0,BUFF_SIZE);
        write(1,"\nClientB> ",10);
        read(0,buff_tx,BUFF_SIZE);
        if(strlen(buff_tx)<2){
            continue;
        }
        send(sockfd,buff_tx,strlen(buff_tx),0);
        


        recv(sockfd,buff_rx,BUFF_SIZE,0);
        write(1,buff_rx,strlen(buff_rx));
        memset(buff_rx,0,BUFF_SIZE);
    }
   
    /* close the socket */
    close(sockfd); 
    return 0;
} 