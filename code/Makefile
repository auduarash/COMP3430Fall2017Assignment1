

#
#    Name: Abdul-Rasheed Audu
#    Student Number: 7779308
#    File Name: Makefile
#    Purpose: Contains declarations for methods used by a server
#                and it's numerous clients for encoding, sending 
#                and decoding messages.
#

CC=gcc
CFLAGS=-Wall

run_q1: q1
	./q1 lynx lolcats.com

q1: qusetion1.o
	$(CC) $(CFLAGS) question1.o -o q1

qusetion1.o: question1.c
	$(CC) $(CFLAGS) -c question1.c

clean_q1:
	rm -rf q1 raw.log

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
	killall server || rm -rf primeserver

clean_objects:
	rm -rf *.o 

clean_executables:
	rm -rf main server client primeserver

clean:
	make clean_client && make clean_server && make clean_objects && make clean_executables && make clean_q1