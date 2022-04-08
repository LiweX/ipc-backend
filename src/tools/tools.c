#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include "tools.h"

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
                write(1,"Transfer complete...\n",21);
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

    FILE *fp = fopen(filename, "ab");
    if(NULL == fp)
    {
        printf("Error opening file");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        bzero(recvBuff,1024);
        size_t bytes = (size_t)read(sockfd,recvBuff,1024);
        if(bytes>0) fwrite(recvBuff, 1,bytes,fp);
        if(bytes<1024){
            write(1,"Transfer complete...\n",21);
            break;
        } 
    }
    fclose(fp);
}

void prepare_pool(sqlite3 **db,char * dbname,int* flags){

    for(int i=0;i<POOL_SIZE;i++){
        int r = sqlite3_open(dbname,&db[i]);
        flags[i]=0;
        if(r!=SQLITE_OK){
        fprintf(stderr, "Open error: %s\n", sqlite3_errmsg(db[i]));
        exit(EXIT_FAILURE);
        }
    }
}

int get_db_index(int* flags){
    for(int i=0;i<POOL_SIZE;i++){
        if(flags[i]==0){
            flags[i]=1;
            return i;
        }
    }
    return 5;
}

sqlite3 * get_db(sqlite3 ** pool , int * flags,int *n_db){
    while(1){
        *n_db = get_db_index(flags);
        if(*n_db == 5){
            continue;
        }else{
            flags[*n_db]=1;
            break;   
        }
    }
    return pool[*n_db];
}

void release_db(int n_db,int* flags){
    flags[n_db]=0;
}