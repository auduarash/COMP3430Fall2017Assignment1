/**
    Name: Abdul-Rasheed Audu
    Student Number: 7779308
    File Name: protocol.h
    Purpose: Contains declarations for methods used by a server
                and it's numerous clients for encoding, sending 
                and decoding messages.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


#include <string.h>
#include <unistd.h>
#define MAX_BUF 1024
#define PRIME_FIFO "./primeserver"
int server_fifo_fd;



/**
    Message info struct

*/
typedef struct MESSAGE_INFO *message_info;
struct MESSAGE_INFO {
    int client_number; //client who sent message. -1 if not needed receives the message
    int prime_number; //-1 if number is not prime. Otherwise prime is given
};
message_info new_message();

message_info decode_message(char buf[], int message_length);

/*
    Server methods
*/
message_info decode_server_message(char buf[], int message_length);
int extract_prime_from_message(char buf[], int message_length);
void send_number_to_client(int number, int client_id);

/*
    Client methods
*/
message_info decode_client_message(char buf[], int message_length);
void send_prime_request_to_server(int client_id);
void send_prime_number_to_server(int prime_number, int client_id);

int is_number_prime(int number);

void get_client_fifo_name_for_client_id(char client_name[], int id);