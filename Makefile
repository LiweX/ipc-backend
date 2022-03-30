CFLAGS=-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11

all: directories server client createdb testdb

directories:
	mkdir -p bin/client
	mkdir -p bin/server
	mkdir -p bin/tools
	mkdir -p obj

server: src/server/mainserver.c obj/tools.o obj/serverA.o obj/serverB.o obj/serverC.o obj/sqlite3.o
	gcc $(CFLAGS) src/server/mainserver.c -o bin/server/server obj/serverA.o obj/serverB.o obj/serverC.o obj/sqlite3.o obj/tools.o -ldl -pthread

client: src/client/mainclient.c obj/clientA.o obj/clientB.o obj/clientC.o obj/tools.o
	gcc $(CFLAGS) src/client/mainclient.c -o bin/client/client obj/clientA.o obj/clientB.o obj/clientC.o obj/tools.o

obj/serverA.o: src/server/serverA.c src/server/serverA.h
	gcc $(CFLAGS) -c src/server/serverA.c -o obj/serverA.o

obj/clientA.o: src/client/clientA.c src/client/clientA.h
	gcc $(CFLAGS) -c src/client/clientA.c -o obj/clientA.o

obj/serverB.o: src/server/serverB.c src/server/serverB.h
	gcc $(CFLAGS) -c src/server/serverB.c -o obj/serverB.o

obj/clientB.o: src/client/clientB.c src/client/clientB.h
	gcc $(CFLAGS) -c src/client/clientB.c -o obj/clientB.o

obj/serverC.o: src/server/serverC.c src/server/serverC.h
	gcc $(CFLAGS) -c src/server/serverC.c -o obj/serverC.o

obj/clientC.o: src/client/clientC.c src/client/clientC.h
	gcc $(CFLAGS) -c src/client/clientC.c -o obj/clientC.o

obj/tools.o: src/tools/tools.c src/tools/tools.h
	gcc $(CFLAGS) -c src/tools/tools.c -o obj/tools.o

createdb: src/tools/createdb.c obj/sqlite3.o
	gcc src/tools/createdb.c -o bin/tools/createdb obj/sqlite3.o -ldl -pthread

testdb: src/tools/testdb.c obj/sqlite3.o
	gcc src/tools/testdb.c -o bin/tools/testdb obj/sqlite3.o -ldl -pthread

obj/sqlite3.o: src/tools/sqlite3.c src/tools/sqlite3.h
	gcc -c src/tools/sqlite3.c -o obj/sqlite3.o

clean:
	rm -f obj/*.o bin/client/client bin/server/server bin/tools/*