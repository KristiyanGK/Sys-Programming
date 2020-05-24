#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <arpa/inet.h>
#include "coordinates.h"
#include "client.h"

#define PORT 10000
#define SERVER_ADDRESS "127.0.0.1"

void receive_struct(coordinates* data);
void end_communication();
void setup_sigaction();
void handle_sigint(int sig);

int sockfd;

int main(int argc, char *argv[]) {
    char command[BUFF_SIZE];
    coordinates data = {0};

    setup_sigaction();

    sockfd = setup_connection(SERVER_ADDRESS, PORT);

    for (;;) {
        memset(&command, 0, sizeof(command));

        fgets(command, sizeof(command), stdin);
        strtok(command, "\n");

        if(strcmp(command, END) == 0) {
            end_communication();
            break;
        } 
        else if (strcmp(command, GET) == 0) {
            receive_struct(&data);
            printf("-------------------\n");
            printf("Received struct with values: x=%d, y=%d\n", data.x, data.y);
            printf("-------------------\n");
        }
        else {
            printf("Invalid command!\n");
        }
    }

    client_close(sockfd);
    
    return 0;
}

void receive_struct(coordinates* data) {
    char buffer[BUFF_SIZE];
    uint32_t x, y;
    ssize_t len;

    strcpy(buffer, "get");

    client_send(sockfd, buffer, sizeof(buffer));

    client_recv(sockfd, &x, sizeof(uint32_t));

    client_recv(sockfd, &y, sizeof(uint32_t));

    data->x = ntohl(x);
    data->y = ntohl(y);
}

void end_communication() {
    char buffer[BUFF_SIZE];

    strcpy(buffer, "end");

    client_send(sockfd, buffer, sizeof(buffer));
}

void handle_sigint(int sig) {
    end_communication();
    exit(0);
}

void setup_sigaction() {
    struct sigaction action = {0};

    sigemptyset(&action.sa_mask);

    action.sa_handler = handle_sigint;
    action.sa_flags = SA_NODEFER;

    sigaction(SIGINT, &action, NULL);
}
