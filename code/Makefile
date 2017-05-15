CC=gcc
CFLAGS=-Wall


server: primeserver.o
	$(CC) $(CFLAGS) primeserver.o -o server

primeserver.o: primeserver.c
	$(CC) $(CFLAGS) -c primeserver.c 

run_server: server
	echo "Starting server" && ./server

client: primeclient.o
	$(CC) $(CFLAGS) primeclient.o -o client

primeclient.o: primeclient.c
	$(CC) $(CFLAGS) -c primeclient.c 

run_client: client
	for x in $$(seq 0 9); do ./client -c $$x & done

clean_client: 
	killall client || true

clean_server:
	killall server || true

clean_objects:
	rm -rf *.o 

clean_executables:
	rm -rf main server client primeserver

clean:
	make clean_client && make clean_server && make clean_objects && make clean_executables