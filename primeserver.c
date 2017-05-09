#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define MAX_BUF 1024
#define PRIME_FIFO "./primeserver"

void sig_int_handler(int signum) {
    printf("Received sig int %d. Now terminating.\n", signum);
    unlink(PRIME_FIFO);
    exit(EXIT_SUCCESS);
}

void sig_term_handler(int signum) {
    printf("Received kill messaged %d. Now terminating.\n", signum);
    unlink(PRIME_FIFO);
    exit(EXIT_SUCCESS);
}

int main() {

    signal(SIGINT, sig_int_handler);
    signal(SIGTERM, sig_term_handler);


    int fd;
    char buf[MAX_BUF];

    int lastCheckedNumber = 1;
    int largestPrime = 1;

    mkfifo(PRIME_FIFO, 0666);

    fd = open(PRIME_FIFO, O_RDONLY);
    printf("I am a blocking bad boy\n");
    while ( read(fd, buf, MAX_BUF) > 0 ) {
        printf("I am a Sabaton\n");
    }

    close(fd);

    return 0;

}