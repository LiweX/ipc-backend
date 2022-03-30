int callback(void *data, int argc, char **argv, char **azColName);
void sendFile(char * filename , int connfd);
void recvFile(char * filename , int sockfd);