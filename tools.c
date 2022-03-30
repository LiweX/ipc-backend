#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>

void sendFile(char * filename , int connfd){

    write(1,"Starting transfer...\n",21);
    FILE * fp = fopen(filename,"rb");
    if(NULL == fp)
    {
        write(1,"Error opening file",18);
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        unsigned char buff[1024]={0};
        bzero(buff,1024);
        size_t nread = fread(buff,1,1024,fp);
        if(nread>0) write(connfd, buff, nread);
        if (nread < 1024)
        {
            if (feof(fp))
            {
                write(connfd,"Todo OK!",8);
            }
            if (ferror(fp)) write(1,"Error reading\n",14);
            break;
        }
    }
    fclose(fp);
}

void recvFile(char *filename ,int sockfd){

    char recvBuff[1024];
    ssize_t bytes = 0;

    FILE *fp = fopen(filename, "ab");
    if(NULL == fp)
    {
        printf("Error opening file");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        bytes = read(sockfd,recvBuff,1024);
        fwrite(recvBuff, 1,(size_t)bytes,fp);
        if(bytes<1024) break;
    }
    if(bytes < 0)
    {
        printf("\n Read Error \n");
    }
    fclose(fp);
}

