#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#define MAX_BUF 1024
#define PRIME_FIFO "./primeserver"
#define CLIENT_FIFO "./primeclient"
char client_fifo_name[15];
int prime_server_fd;
int prime_client_fd;
int client_id;



void handle_message(char buf[], int message_size);
int number_is_prime(int number);
void print_invalid_arguments(char *program_executable);
void parse_buffer_messages(char buf[], int message_size);
void send_prime_request_to_server();
void extract_prime_and_validate(char number[], int number_length);
void inform_server_number_is_prime(int number);


void print_invalid_arguments(char *program_executable){
    fprintf(stderr, "Usage: %s [-c clientNum]\n", program_executable);
    exit(EXIT_FAILURE);
}


void signal_handler(int signum) {
    printf("Received signal %d. Now terminating.\n", signum);
    char client_name[] = CLIENT_FIFO;
    client_name[13] = client_id + '0';
    close(prime_server_fd);
    close(prime_client_fd);
    unlink(client_name);
    exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[]) {

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    int flags, opt;

    client_id = 0;
    flags = 0;

    while ((opt = getopt(argc, argv, "c:")) != -1){
        switch (opt) {
            case 'c':
                client_id = atoi(optarg);
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
    client_fifo_name[13] = '0'+client_id;

    printf("Client pipe name is %s.\n", client_fifo_name);
    
    char buf[MAX_BUF];

    // printf("Opening server \n");
    prime_server_fd = open(PRIME_FIFO, O_WRONLY);
    // printf("Server opened \n");

    /**
    We create our client pipe and then send a request to the server.
    Since we are reading, we expect the server to send us our first message
    before or after we open. After that, we read the messages we are sent.
    */
    // printf("Check 3 \n");
    send_prime_request_to_server();
    // printf("Check 4\n");
    mkfifo(client_fifo_name, 0666);
    // printf("Check 5\n");
    prime_client_fd = open(client_fifo_name, O_RDWR);
    // printf("Check 6\n");
    int server_message_size;
    while ( (server_message_size = read(prime_client_fd, buf, MAX_BUF)) > 0 ) {
        char * incoming_message = "Message came to client \n";
        // write(STDOUT_FILENO, incoming_message, strlen(incoming_message));
        // write(STDOUT_FILENO, buf, server_message_size);
        // write(STDOUT_FILENO, "\n", 1);
        parse_buffer_messages(buf, server_message_size);
        // printf("Done parsing\n");
        send_prime_request_to_server();
    }


    return 0;
}


void send_prime_request_to_server() {
    char prime_request[9];
    strcpy(prime_request, "!#R?");
    prime_request[1] = client_id + '0';
    // printf("Sending message %s \n", prime_request);
    write(prime_server_fd, prime_request, 4);
}

void inform_server_number_is_prime(int prime_number) {
    char server_buffer[20];
    memset(server_buffer, 0, sizeof(server_buffer));
    strcpy(server_buffer, "!#P");
    sprintf(server_buffer+3, "%d", prime_number);
    int server_size = 0;
    while (server_buffer[server_size++] != 0);
    server_buffer[server_size-1] = '?';
    server_buffer[1] = client_id + '0';
    // printf("Sending message %s \n", server_buffer);
    write(prime_server_fd, server_buffer, server_size);
}

void parse_buffer_messages(char buf[], int message_size) {
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
    char server_id = buf[1];
    //validate_client_number
    int command = buf[2];
    if (server_id == 'S' && command == 'Q') {
        extract_prime_and_validate(buf+3, message_size-4);
    } else {
        // Invalid command
    }
}



void extract_prime_and_validate(char number[], int number_length) {
    char number_ext[number_length+1];
    int i;
    for (i = 0; i < number_length; i++) {
        number_ext[i] = number[i];
    }
    number_ext[number_length] = '\0';
    int num_to_check = atoi(number_ext);

    int isPrime = number_is_prime(num_to_check);
    if (isPrime) {
        inform_server_number_is_prime(num_to_check);
    } else {
        //Do nothing. Number is not prime
    }
}


int number_is_prime(int number) {
    int factors = 0;
    int i;
    for (i = 1; i <= number; i++) {
        if (number % i == 0) factors += 1;
    }
    return  factors==2 ? 1 : 0;
}