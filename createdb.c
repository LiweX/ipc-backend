#include "sqlite3.h"  
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void createdb(char * dbname);

int main(int argc, char* argv[]){
    if(argc!=2) {
        printf("ERROR");
        exit(EXIT_FAILURE);
    }
    createdb(argv[1]);
    exit(EXIT_SUCCESS);
}
                                                                          
void createdb(char * dbname) {

    sqlite3 *db;
    char *err_msg = 0;
    char aux[100];
    bzero(aux,100);
    sprintf(aux,"%s.db",dbname);

    int rc = sqlite3_open(aux, &db);
    if(rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        exit(EXIT_FAILURE);
    }
    char *sql = "DROP TABLE IF EXISTS Cars;"
                "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);"
                "INSERT INTO Cars VALUES(1, 'Audi', 52642);"
                "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);"
                "INSERT INTO Cars VALUES(3, 'Skoda', 9000);"
                "INSERT INTO Cars VALUES(4, 'Volvo', 29000);"
                "INSERT INTO Cars VALUES(5, 'Bentley', 350000);"
                "INSERT INTO Cars VALUES(6, 'Citroen', 21000);"
                "INSERT INTO Cars VALUES(7, 'Hummer', 41400);"
                "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);

        exit(EXIT_FAILURE);
    }
    sqlite3_close(db);
    printf("Base de datos creada...\n");
}