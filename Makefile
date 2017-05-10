CC=gcc


server: primeserver.o
	gcc -Wall primeserver.o -o server

primeserver.o: primeserver.c
	gcc -Wall -c primeserver.c 

run_server: server
	echo "Starting server" && ./server

client: primeclient.o
	gcc -Wall primeclient.o -o client

primeclient.o: primeclient.c
	gcc -Wall -c primeclient.c 

run_client: client
	echo "Starting client" && ./client -c 0