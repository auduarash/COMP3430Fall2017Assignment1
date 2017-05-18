CC=gcc
CFLAGS=-Wall

q1: main.o
	$(CC) $(CFLAGS) main.o -o q1

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean_q1:
	rm -rf q1

protocol.o: protocol.c protocol.h
	$(CC) $(CFLAGS) -c protocol.c

server: primeserver.o protocol.o
	$(CC) $(CFLAGS) primeserver.o protocol.o -o server

primeserver.o: primeserver.c
	$(CC) $(CFLAGS) -c primeserver.c 

run_server: server
	echo "Starting server" && ./server

client: primeclient.o protocol.o
	$(CC) $(CFLAGS) primeclient.o protocol.o -o client

primeclient.o: primeclient.c
	$(CC) $(CFLAGS) -c primeclient.c 

run_client: client
	for x in $$(seq 0 9); do ./client -c $$x & done

clean_client: 
	killall client || for x in $$(seq 0 9); do rm -rf primeclient$$x; done

clean_server:
	rm -rf primeserver

clean_objects:
	rm -rf *.o 

clean_executables:
	rm -rf main server client primeserver

clean:
	make clean_client && make clean_server && make clean_objects && make clean_executables && make clean_q1