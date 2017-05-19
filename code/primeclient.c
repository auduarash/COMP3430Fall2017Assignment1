/**
    Name: Abdul-Rasheed Audu
    Student Number: 7779308
    File Name: primeclient.c
    Purpose: A process that receives an encoded message from
                a server which contains a prime. This process
                verifies the number is a prime and returns an 
                encoded message to the server.
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "protocol.h"

char client_fifo_name[15] = "./primeclientx";
int client_id;


void signal_handler(int signum) {
    close(server_fifo_fd);
    unlink(client_fifo_name);
    exit(EXIT_SUCCESS);
}

void check_server_fifo_exists(){
    struct stat info;
    if (lstat("./primeserver", &info) != 0){
        perror("There was an error opening server fifo");
        signal_handler(0);
    }

}

void parse_buffer_message(char buf[], int message_size) {
    message_info decoded = decode_message(buf, message_size);
    if (decoded == 0) return;
    else if (decoded->prime_number > 0) {
        send_prime_number_to_server(decoded->prime_number, client_id);
    }
}


void print_invalid_arguments_and_exit(char *program_executable){
    fprintf(stderr, "Usage: %s [-c clientNum]\n", program_executable);
    exit(EXIT_FAILURE);
}




int get_client_id(int argc, char *argv[]) {
    int opt, client_id = -1;
    while ((opt = getopt(argc, argv, "c:")) != -1){
        switch (opt) {
            case 'c':
                client_id = atoi(optarg);
                break;
            default:
                break;

        }
    }
    return client_id;
}


int main(int argc, char *argv[]) {
    signal(0, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGPIPE, signal_handler);

    int client_id = get_client_id(argc, argv);
    if (client_id == -1) { 
        print_invalid_arguments_and_exit(argv[0]);
    }
    client_fifo_name[13] = '0'+client_id;
    char buf[MAX_BUF];

    check_server_fifo_exists();
    if ((server_fifo_fd = open("./primeserver", O_WRONLY)) == -1) {
        perror("Could not open server fifo");
    }
    send_prime_request_to_server(client_id);
    mkfifo(client_fifo_name, 0666);
    int prime_client_fd = open(client_fifo_name, O_RDWR);
    int server_message_size;
    
    while ( (server_message_size = read(prime_client_fd, buf, MAX_BUF)) > 0 ) {
        parse_buffer_message(buf, server_message_size);
        send_prime_request_to_server(client_id);
    }
    return 0;
}

