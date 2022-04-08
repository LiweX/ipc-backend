#include "sqlite3.h"
#define POOL_SIZE 5
void sendFile(char * filename , int connfd);
void recvFile(char * filename , int sockfd);
void prepare_pool(sqlite3 **db,char * dbname,int *flags);
int get_db_index(int* flags);
sqlite3 * get_db(sqlite3 ** pool , int * flags, int * n_db);
void release_db(int n_db,int* flags);