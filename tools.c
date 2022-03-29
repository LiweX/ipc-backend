#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include "tools.h"

void init_dbs(sqlite3 ** dbs){

    int r1 = sqlite3_open("test.db", &dbs[0]);
    int r2 = sqlite3_open("test.db", &dbs[1]);
    int r3 = sqlite3_open("test.db", &dbs[2]);
    int r4 = sqlite3_open("test.db", &dbs[3]);
    int r5 = sqlite3_open("test.db", &dbs[4]);

    if(r1 != SQLITE_OK || r2 != SQLITE_OK || r3 != SQLITE_OK || r4 != SQLITE_OK || r5 != SQLITE_OK) {

        fprintf(stderr, "Cannot open databases: %s\n", sqlite3_errmsg(dbs[0]));
        sqlite3_close(dbs[0]);
        sqlite3_close(dbs[1]);
        sqlite3_close(dbs[2]);
        sqlite3_close(dbs[3]);
        sqlite3_close(dbs[4]);
        
        exit(EXIT_FAILURE);
    }
}