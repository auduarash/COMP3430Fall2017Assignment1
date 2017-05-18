#include "protocol.h"


message_info new_message() {
    message_info new_m = (message_info) malloc(sizeof(struct MESSAGE_INFO));
    new_m->client_number = -1;
    new_m->prime_number = -1;
    return new_m;
}

message_info decode_message(char buf[], int message_length) {
    char sender = buf[1];
    if (sender == 'S') {
        return decode_client_message(buf, message_length);
    } else if ('0' <= sender && sender <= '9') {
        return decode_server_message(buf, message_length);
    } else {
        return 0; //Invalid command
    }
}

message_info decode_client_message(char buf[], int message_length) {
    char command = buf[2];
    if (command == 'Q') {
        buf[message_length-1] = '\0';
        int prime_number = atoi(buf+3);
        int is_prime = is_number_prime(prime_number);
        message_info result = new_message();
        if (is_prime) result->prime_number = prime_number;
        return result;
    } else {
        return 0;
    }
}

message_info decode_server_message(char buf[], int message_length) {
    char command = buf[2];
    int client_id = buf[1] - '0';
    message_info server_message = new_message();
    if (command == 'R') {
        server_message->client_number = client_id;
    } else if (command == 'P') {
        int prime = extract_prime_from_message(buf, message_length);
        server_message->prime_number = prime;
        server_message->client_number = client_id;
    } else {
        server_message = 0;
    }
    return server_message;
}

void send_prime_number_to_server(int prime_number, int client_id) {
    char server_buffer[20];
    sprintf(server_buffer, "!%dP%d?", client_id, prime_number);
    int prime_server_fd = open(PRIME_FIFO, O_WRONLY);
    write(prime_server_fd, server_buffer, strlen(server_buffer));
    close(prime_server_fd);
}

void send_prime_request_to_server(int client_id) {
    char prime_request[5];
    sprintf(prime_request, "!%dR?", client_id);
    int prime_server_fd = open(PRIME_FIFO, O_WRONLY);
    write(prime_server_fd, prime_request, 4);
    close(prime_server_fd);
}

int is_number_prime(int number) {
    int factors = 0;
    int i;
    for (i = 1; i <= number; i++) {
        if (number % i == 0) factors += 1;
    }
    return  factors==2 ? 1 : 0;
}

void send_number_to_client(int number, int client_id) {
    char client_name[15] = "./primeclient#";
    client_name[13] = '0' + client_id;
    char client_buffer[20];
    sprintf(client_buffer, "!SQ%d?", number);

    int client_fd = open(client_name, O_WRONLY);
    write(client_fd, client_buffer, strlen(client_buffer));
    close(client_fd);
}

int extract_prime_from_message(char buf[], int message_length) {
    buf[message_length-1] = '\0';
    int prime = atoi(buf+3);
    return prime;
}