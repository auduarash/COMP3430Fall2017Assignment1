#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_BUF 1024

void print_invalid_arguments(char *program_executable){
    fprintf(stderr, "Usage: %s [-c clientNum]\n", program_executable);
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]) {

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

    printf("Client number is %d.\n", clientNum);
    

    return 0;
}