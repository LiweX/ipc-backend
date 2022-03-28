#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFF_SIZE 1024*2

int clientB(int port, char* address) 
{ 
    int sockfd; 
    struct sockaddr_in servaddr;
    int buff_tx[BUFF_SIZE];
    int buff_rx[BUFF_SIZE];

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
    
    printf("connected to the server..\n"); 
  
    /* send test sequences*/
    while(1){
        
        write(0,"\nClientB> ",100);
        read(0,buff_tx,BUFF_SIZE);
        send(sockfd,buff_tx,strlen(buffer),0);
        recv(sockfd,buff_rx,BUFF_SIZE,0);
        write(0,buff_rx,BUFF_SIZE);

    }
   
    /* close the socket */
    close(sockfd); 
    return 0;
} 