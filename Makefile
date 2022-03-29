CFLAGS=-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11

all: server client

server: mainserver.c serverA.o serverB.o serverC.o sqlite3.o tools.o
	gcc $(CFLAGS) mainserver.c -o server serverA.o serverB.o serverC.o sqlite3.o tools.o -ldl -pthread

client: mainclient.c clientA.o clientB.o clientC.o
	gcc $(CFLAGS) mainclient.c -o client clientA.o clientB.o clientC.o

test1: sqltest1.c sqlite3.o
	gcc sqltest1.c -o test sqlite3.o -ldl -pthread

test2: sqltest2.c sqlite3.o
	gcc sqltest2.c -o test2 sqlite3.o -ldl -pthread

sqlite3.o: sqlite3.c sqlite3.h
	gcc -c sqlite3.c -o sqlite3.o

serverA.o: serverA.c serverA.h
	gcc $(CFLAGS) -c serverA.c

clientA.o: clientA.c clientA.h
	gcc $(CFLAGS) -c clientA.c

serverB.o: serverB.c serverB.h
	gcc $(CFLAGS) -c serverB.c

clientB.o: clientB.c clientB.h
	gcc $(CFLAGS) -c clientB.c

serverC.o: serverC.c serverC.h
	gcc $(CFLAGS) -c serverC.c

clientC.o: clientC.c clientC.h
	gcc $(CFLAGS) -c clientC.c

tools.o: tools.c tools.h
	gcc $(CFLAGS) -c tools.c

clean:
	rm -f *.o client server