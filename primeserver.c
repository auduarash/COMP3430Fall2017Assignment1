#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 1024
#define PRIME_FIFO "./primeserver"
#define CLIENT_FIFO "./primeclient5"

void signal_handler(int signum) {
    printf("Received signal %d. Now terminating.\n", signum);
    unlink(PRIME_FIFO);
    exit(EXIT_SUCCESS);
}

void send_message_to_client(int client_number) {
    char client_name[15];
    // printf("Check server 1\n");
    strcpy(client_name, "./primeclient5");
    // printf("Check server 2\n");
    int client_fd = open(client_name, O_WRONLY);
    // printf("Check server 3\n");
    write(client_fd, "!5Q123?", 7);
    // printf("Check server 4\n");
    close(client_fd);
}

int main() {

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);


    int fd;
    char buf[MAX_BUF];

    int lastCheckedNumber = 1;
    int largestPrime = 1;

    mkfifo(PRIME_FIFO, 0666);

    fd = open(PRIME_FIFO, O_RDWR);
    printf("I am a blocking bad boy\n");
    while ( read(fd, buf, MAX_BUF) > 0 ) {
        printf("Message came to server %s %d \n", buf, lastCheckedNumber++);
        send_message_to_client(5);
    }

    close(fd);

    return 0;

}