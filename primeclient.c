#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUF 1024
#define PRIME_FIFO "./primeserver"
char client_fifo_name[15];
int prime_server_fd;
int prime_client_fd;

void print_invalid_arguments(char *program_executable){
    fprintf(stderr, "Usage: %s [-c clientNum]\n", program_executable);
    exit(EXIT_FAILURE);
}

void signal_handler(int signum) {
    printf("Received signal %d. Now terminating.\n", signum);
    unlink("./primeclient5");
    close(prime_server_fd);
    exit(EXIT_SUCCESS);
}


void send_prime_request_to_server(int client_number);

int main(int argc, char *argv[]) {

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    int flags, opt;
    int clientNum;

    clientNum = 0;
    flags = 0;

    while ((opt = getopt(argc, argv, "c:")) != -1){
        switch (opt) {
            case 'c':
                clientNum = atoi(optarg);
                flags = 1;
                break;
            default:
                break;

        }
    }
    if (flags == 0) { 
        print_invalid_arguments(argv[0]);
    }
    char *temp_fifo = "./primeclientx";
    strcpy(client_fifo_name, temp_fifo);
    client_fifo_name[13] = '0'+clientNum;

    printf("Client pipe name is %s.\n", client_fifo_name);
    
    char buf[MAX_BUF];

    printf("Opening server \n");
    prime_server_fd = open(PRIME_FIFO, O_WRONLY);
    printf("Server opened \n");

    /**
    We create our client pipe and then send a request to the server.
    Since we are reading, we expect the server to send us our first message
    before or after we open. After that, we read the messages we are sent.
    */
    printf("Check 3 \n");
    send_prime_request_to_server(clientNum);
    printf("Check 4\n");
    mkfifo(client_fifo_name, 0666);
    printf("Check 5\n");
    prime_client_fd = open(client_fifo_name, O_RDWR);
    printf("Check 6\n");

    while ( read(prime_client_fd, buf, MAX_BUF) > 0 ) {
        printf("Check 7\n");
        int data_length = 1;
        while (buf[data_length - 1] != '?') {
            data_length += 1;
            printf("Didn't find \n");
        }
        //verify_message that came in from the server
        printf("Data came in %s \n", buf);
        send_prime_request_to_server(clientNum);
    }


    return 0;
}


void send_prime_request_to_server(int client_number) {
    char prime_request[5];
    strcpy(prime_request, "!#R?");
    prime_request[1] = client_number;
    write(prime_server_fd, prime_request, 4);
}