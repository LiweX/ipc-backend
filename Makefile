CFLAGS=-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11

test: sqltest.c sqlite3.o
	gcc sqltest.c -o test sqlite3.o -ldl -pthread

sqlite3.o: sqlite3.c sqlite3.h
	gcc -c sqlite3.c -o sqlite3.o

clean:
	rm -f *.o