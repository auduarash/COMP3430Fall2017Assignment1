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
void send_next_prime_to_client(int client_number);
void print_number_is_prime(char buf[], int number_length);


void signal_handler(int signum) {
    unlink(PRIME_FIFO);
    exit(EXIT_SUCCESS);
}

void parse_buffer_messages(char buf[], int message_size) {
    printf("Parsing %s %d \n", buf, num_to_check);
    int index = 0;
    while (index < message_size) {
        if ( buf[index] == '!' ) { //start of message
            int messageStart = index;
            while (buf[index] != '?' && index < message_size) {
                index++;
            }
            if (index < message_size) {
                handle_message(buf+messageStart, index-messageStart+1);
            } else {
                //print_invalid_message_from_client
            }
        }
        index++;
    }
}

void handle_message(char buf[], int message_size) {
    int client_id = buf[1] - '0';
    //validate_client_number
    int command = buf[2];
    if (command == 'R') {
        send_next_prime_to_client(client_id);
    } else if ( command == 'P' ) {
        print_number_is_prime(buf+3, message_size-4);

    }
}

void send_next_prime_to_client(int client_number) {
    num_to_check = num_to_check + 1;
    if (highest_prime > num_to_check) {
        num_to_check = highest_prime + 1;
    }
    char client_name[15];
    // printf("Check server 1\n");
    strcpy(client_name, "./primeclient#");
    client_name[13] = '0' + client_number;
    // printf("Check server 2\n");
    int client_fd = open(client_name, O_WRONLY);
    char client_buffer[20];
    memset(client_buffer, 0, sizeof(client_buffer));
    strcpy(client_buffer, "!SQ");
    sprintf(client_buffer+3, "%d", num_to_check);
    int client_size = 0;
    while (client_buffer[client_size++] != 0);
    client_buffer[client_size-1] = '?';
    // write(STDOUT_FILENO, client_buffer, client_size);
    // write(STDOUT_FILENO, "\n", 1);
    write(client_fd, client_buffer, client_size);
    close(client_fd);
}

void print_number_is_prime(char buf[], int number_length) {
    char user_number [number_length+1];
    int i;
    for (i = 0; i < number_length; i++) {
        user_number[i] = buf[i];
    }
    user_number[number_length] = '\0';
    int num = atoi(user_number);
    if (num > highest_prime) {
        highest_prime = num;
        printf("%d is Prime\n", highest_prime);
    }
}

int main(int argc, char *argv[]) {

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);


    int fd;
    char buf[MAX_BUF];
    int client_message_size;


    mkfifo(PRIME_FIFO, 0666);

    fd = open(PRIME_FIFO, O_RDWR);
    while ( (client_message_size = read(fd, buf, MAX_BUF)) > 0 ) {
        // char * incoming_message = "Message came to server ";
        // write(STDOUT_FILENO, incoming_message, strlen(incoming_message));
        // write(STDOUT_FILENO, buf, client_message_size);
        // write(STDOUT_FILENO, "\n", 1);
        parse_buffer_messages(buf, client_message_size);
    }

    close(fd);

    return 0;

}