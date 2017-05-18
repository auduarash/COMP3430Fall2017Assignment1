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
#include <fcntl.h>



int main(int argc, char *argv[]){
    int pipefd[2];
    pid_t cpid;
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
        //exit if exec fails
        if (execvp(argv[1], &argv[1]) < 0){
            fprintf(stderr, "Error: exec failed\n");
            exit(EXIT_FAILURE);
        }

    } else { 
        int current_state = 0;
        char one;
        char cat_buffer[3] = {0, 0, 0};
        int cat_buffer_size = 0;

        close(pipefd[1]);//we are not using the write end of the pipe

        FILE* raw_file = fopen("./raw.log", "w");

        while (read(pipefd[0], &one, 1) > 0){

            /*
                State machine states
                0 - Invalid state
                1 - Last character was a c
                2 - Last character was an a

                If c and|or a are entered, cat_buffer is used to store both
                characters in the event that the word does not end up being 
                cat. In which case, either c or a is output.
                The buffer is also necessary since we can't tell if the c
                or the a is upper or lower case
            */
            fprintf(raw_file, "%c", one);
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

                if (current_state != 0){
                    current_state = 0;
                    write(STDOUT_FILENO, cat_buffer, cat_buffer_size);
                    cat_buffer_size = 0;
                }
                write(STDOUT_FILENO, &one, 1);

            }
        }
        close(pipefd[0]); //we are done with the read end of the pipe
        fclose(raw_file);

        while (wait(NULL) != cpid); //wait for child process to exit

        exit(EXIT_SUCCESS);
    }

}