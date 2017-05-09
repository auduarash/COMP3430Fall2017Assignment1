/**
NAME: Abdul-Rasheed Audu
Student Number: 7779308
Course: COMP 3430 - Operating Systems
Topic: Assignment 1
Description: //TODO
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



int main(int argc, char *argv[]){
    int pipefd[2];
    pid_t cpid;
    char one;
    char cat_buffer[3] = {0, 0, 0};
    int cat_buffer_size = 0;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <string> [<args>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) { //Child process runs here
        dup2(pipefd[1], STDOUT_FILENO);
        if (execvp(argv[1], &argv[1]) < 0){
            fprintf(stderr, "Error: exec failed\n");
            exit(EXIT_FAILURE);
        }
    } else { //Parent process runs here
        /*
            State machine states
            0 - Invalid state
            1 - Last character was a c
            2 - Last character was an a
        */
        int current_state = 0;
        close(pipefd[1]);
        while (read(pipefd[0], &one, 1) > 0){
            if (one == 'c' || one == 'C'){
                current_state = 1;
                cat_buffer[0] = one;
                cat_buffer_size = 1;
            } else if ((one == 'a' || one == 'A') && current_state == 1) {
                current_state = 2;
                cat_buffer[1] = one;
                cat_buffer_size = 2;
            } else if ((one == 't' || one == 'T') && current_state == 2) {
                //TODO find a way to do what he wants
                write(STDOUT_FILENO, "dog", 3);
                current_state = 0;
            } else {
                //PRINT DOG
                if (current_state != 0){
                    current_state = 0;
                    write(STDOUT_FILENO, cat_buffer, cat_buffer_size);
                    cat_buffer_size = 0;
                }
                write(STDOUT_FILENO, &one, 1);
            }
        }
        close(pipefd[0]);
        while (wait(NULL) != cpid);
        exit(EXIT_SUCCESS);
    }

}