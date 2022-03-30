#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <net/if.h>
#include "tools.h"

#define BUFF_SIZE 1024

int clientC(int port,char*address,char*interface) 
{ 
    char dbname[100];
    bzero(dbname,100);
    write(1,"Nombre del backup: ",19);
    ssize_t bytes = read(0,dbname,100);
    dbname[bytes-1]='\0';
    strcat(dbname,".db");
    write(1,dbname,strlen(dbname));

    int sockfd; 
    struct sockaddr_in6 servaddr; 
    char *request="Pasame la base de datos\n";
    char buff_rx[BUFF_SIZE];
    bzero(buff_rx,BUFF_SIZE);
    /* Socket creation */
    sockfd = socket(PF_INET6, SOCK_STREAM, 0); 
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
    servaddr.sin6_family = AF_INET6; 
    servaddr.sin6_port = htons((uint16_t)port);
    inet_pton(AF_INET6, address, &servaddr.sin6_addr);
    servaddr.sin6_scope_id = if_nametoindex(interface);
  
    /* try to connect the client socket to server socket */
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) 
    { 
        printf("connection with the server failed...\n");  
        return -1;
    } 
    
    recv(sockfd,buff_rx,BUFF_SIZE,0);
    write(1,buff_rx,strlen(buff_rx));
    write(1,"procedo a pedir la base de datos..\n",35);

    send(sockfd,request,strlen(request),0);

    recvFile(dbname,sockfd);


    close(sockfd);
    
    exit(EXIT_SUCCESS);
} 