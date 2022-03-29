#include "sqlite3.h"
void init_dbs(sqlite3 **dbs);
int callback(void *data, int argc, char **argv, char **azColName);