CFLAGS=-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11

test1: sqltest1.c sqlite3.o
	gcc sqltest1.c -o test sqlite3.o -ldl -pthread

test2: sqltest2.c sqlite3.o
	gcc sqltest2.c -o test2 sqlite3.o -ldl -pthread

sqlite3.o: sqlite3.c sqlite3.h
	gcc -c sqlite3.c -o sqlite3.o

serverA.o: serverA.c serverA.h


clean:
	rm -f *.o