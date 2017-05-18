#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "protocol.h"

#define MAX_BUF 1024
#define PRIME_FIFO "./primeserver"
#define CLIENT_FIFO "./primeclient5"

int num_to_check = 1;
int highest_prime = 1;

void handle_message(char buf[], int message_size);
void signal_handler(int signum);
void parse_buffer_messages(char buf[], int message_size);


void signal_handler(int signum) {
    unlink(PRIME_FIFO);
    exit(EXIT_SUCCESS);
}

void parse_buffer_messages(char buf[], int message_size) {
    //printf("Parsing %s %d \n", buf, num_to_check);
    int index = 0;
    while (index < message_size) {
        if ( buf[index] == '!' ) { //start of message
            int messageStart = index;
            while (buf[index] != '?' && index < message_size) {
                index++;
            }
            if (index < message_size) {
                handle_message(buf+messageStart, index-messageStart+1);
            }
        }
        index++;
    }
}

void handle_message(char buf[], int message_size) {
    message_info info = decode_message(buf, message_size);
    if (info) {
        if (info->prime_number > 0){
            if (info->prime_number > highest_prime) {
                highest_prime = info->prime_number + 1;
                printf("%d is a prime number\n", info->prime_number);
            }
        } else {
            send_number_to_client(num_to_check++, info->client_number);
        }
    }
}


int main(int argc, char *argv[]) {

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGPIPE, signal_handler);


    int fd;
    char buf[MAX_BUF];
    int client_message_size;


    mkfifo(PRIME_FIFO, 0666);

    fd = open(PRIME_FIFO, O_RDWR);
    while ( (client_message_size = read(fd, buf, MAX_BUF)) > 0 ) {
        parse_buffer_messages(buf, client_message_size);
    }

    close(fd);

    return 0;

}