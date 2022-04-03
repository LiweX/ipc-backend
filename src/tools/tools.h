#include "sqlite3.h"
#define POOL_SIZE 5
void sendFile(char * filename , int connfd);
void recvFile(char * filename , int sockfd);
void prepare_pool(sqlite3 **db,char * dbname,int *flags);
int get_db(int* flags);