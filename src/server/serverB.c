#include <unistd.h>  
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "../tools/sqlite3.h"
#include "../tools/tools.h"
#include <time.h>

/* server parameters */
#define BUFF_SIZE       1024*2              /* Buffer rx, tx max size  */
#define BACKLOG         5                 /* Max. client pending connections  */

char buff_rxB[BUFF_SIZE];   /* buffers for reception  */
char buff_txB[BUFF_SIZE];   /* buffers for reception  */

int callbackB(void *data, int argc, char **argv, 
                    char **azColName);

int serverB(int port, char* address, sqlite3 * db)          /* input arguments are not used */
{ 
    int sockfd;  /* listening socket and connection socket file descriptors */
    unsigned int len;     /* length of client address */
    int n_con=0;
    struct sockaddr_in servaddr, client; 
    long int len_rx;                     /* received and sent length, in bytes */
    bzero(buff_rxB,BUFF_SIZE);
    bzero(buff_txB,BUFF_SIZE);
    char *aux = (char *)malloc(sizeof(char)*BUFF_SIZE);

    /* socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) 
    { 
        fprintf(stderr, "[IPV4_TCP_SERVER-error]: socket creation failed. %d: %s \n", errno, strerror( errno ));
        exit(EXIT_FAILURE);
    } 
    else
    {
        printf("[IPV4_TCP_SERVER]: Socket successfully created..\n"); 
    }
    int flag = 1 ;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1) {
        fprintf(stderr, "[IPV4_TCP_SERVER-error]: socket creation failed. %d: %s \n", errno, strerror( errno ));
		exit(EXIT_FAILURE);
	}
    
    /* clear structure */
    memset(&servaddr, 0, sizeof(servaddr));
  
    /* assign IP, port, IPV4 */
    servaddr.sin_family      = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(address); 
    servaddr.sin_port        = htons((uint16_t)port);

    
    /* Bind socket */
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) 
    { 
        fprintf(stderr, "[IPV4_TCP_SERVER-error]: socket bind failed. %d: %s \n", errno, strerror( errno ));
        exit(EXIT_FAILURE);
    } 
    else
    {
        printf("[IPV4_TCP_SERVER]: Socket successfully binded \n");
    }
  
    /* Listen */
    if ((listen(sockfd, BACKLOG)) != 0) 
    { 
        fprintf(stderr, "[IPV4_TCP_SERVER-error]: socket listen failed. %d: %s \n", errno, strerror( errno ));
        exit(EXIT_FAILURE);
    } 
    else
    {
        printf("[IPV4_TCP_SERVER]: Listening on port %d \n\n", ntohs(servaddr.sin_port) ); 
    }
    len = sizeof(client);
    int r;
    while(1)
    {
        int connfd = accept(sockfd, (struct sockaddr *)&client, &len);
        n_con++;
        int pid = fork();
        if (pid==0)
        {   
             
            if (connfd < 0) 
            { 
                fprintf(stderr, "[IPV4_TCP_SERVER-error]: connection not accepted. %d: %s \n", errno, strerror( errno ));
                n_con--;
                exit(EXIT_FAILURE);       
            } 
            else
            {   
                send(connfd,"Connected to the server...\n",27,0);
                char logtime[100];
                time_t rawtime;
                while(1) /* read data from a client socket till it is closed */ 
                {     
                    len_rx = recv(connfd, buff_rxB, BUFF_SIZE,0); // data reception
                    
                    if(len_rx == -1)
                    {
                        fprintf(stderr, "[IPV4_TCP_SERVER-error]: connfd cannot be read. %d: %s \n", errno, strerror( errno ));
                    }
                    else if(len_rx == 0) /* if length is 0 client socket closed, then exit */
                    {
                        sprintf(aux,"[IPV4_TCP_SERVER]: client %d socket closed \n\n",n_con);
                        write(1,aux,strlen(aux));
                        close(connfd);
                        exit(EXIT_SUCCESS);
                    }
                    else
                    {   
                        bzero(logtime,100);
                        struct tm * timeinfo;
                        time(&rawtime);
                        timeinfo = localtime(&rawtime);
                        strftime(logtime,99,"%d/%m/%y - %H:%M:%S",timeinfo);
                        buff_rxB[len_rx-1]='\0';

                        char * err_msg=0;

                        sprintf(aux,"insert into log values(\"%s\",\"%s\")",buff_rxB,logtime);

                        sqlite3_exec(db,aux,0,0,&err_msg);

                        r = sqlite3_exec(db,buff_rxB,callbackB,0,&err_msg);
                        
                        if(r != SQLITE_OK){

                            bzero(buff_txB,BUFF_SIZE);
                            sprintf(buff_txB, "Cannot process query: %s\n", sqlite3_errmsg(db));
                            send(connfd,buff_txB,strlen(buff_txB),0);

                        }
                        if(r == SQLITE_OK){
                            if(strlen(buff_txB)==0)send(connfd," ",1,0);
                            else send(connfd,buff_txB,strlen(buff_txB),0);
                        }

                        bzero(aux,BUFF_SIZE);
                        bzero(buff_rxB,BUFF_SIZE);
                        bzero(buff_txB,BUFF_SIZE);
                    }            
                }  
            } 
        }
    }    
}

int callbackB(void *data, int argc, char **argv, 
                    char **azColName) {
    char aux[10000];
    data=data;
    for (int i = 0; i < argc; i++) {

        sprintf(aux,"%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        strcat(buff_txB,aux);
        memset(aux,0,10000);

    }
    return 0;
}