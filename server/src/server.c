#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "server.h"
#include "coordinates.h"

// private functions declarations
int random_int();
void *handle_connection(void* input);

// global variables
coordinates data = {0};
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// public functions definitions

int create_server(int port, int *serverfd) {
    struct sockaddr_in serveraddr = {0};

    if (port < 0) {
        perror("");
        return E_INVALID_PORT;
    }

    *serverfd = socket(AF_INET, SOCK_STREAM, 0);

    if (*serverfd == -1) {
        perror("");
        return E_FAILED_SOCKET_CREATION;
    }

    // Assign IP, PORT
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);

    if ((bind(*serverfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))) != 0) {
        perror("");
        return E_FAILED_BINDING;
    }

    return E_SUCCESS;
}

int start_listening(int serverfd, int maxConn) {
    int len, connfd;
    struct sockaddr_in cli;
    pthread_t thread;

    if (pthread_mutex_init(&lock, NULL) != 0) {
        perror("");
        return E_FAILED_MUTEX_INIT;
    }

    if((listen(serverfd, maxConn)) != 0) {
        perror("");
        return E_FAILED_LISTENING;
    }

    len = sizeof(cli);

    printf("Server is listening!\n");

    for(;;) {
        connfd = accept(serverfd, (struct sockaddr*)&cli, &len);

        if (connfd < 0) {
            perror("");
            return E_FAILED_SERVER_ACCEPT;
        }

        printf("Received Request from %d\n", connfd);

        pthread_create(&thread, NULL, handle_connection, (void *)&connfd);
    }

    pthread_mutex_destroy(&lock);

    return E_SUCCESS;
}

// private function definitions

void *handle_connection(void* input) {
    int clientfd = *((int*)input);
    uint32_t xSend, ySend;
    char buffer[BUFF_SIZE] = {0};

    for(;;) {
        recv(clientfd, buffer, BUFF_SIZE, 0);

        if (strcmp(buffer, GET) == 0) {
            pthread_mutex_lock(&lock);
            data.x = random_int();
            data.y = random_int();
            pthread_mutex_unlock(&lock);
            
            printf("Server sending struct with values: x=%d, y=%d to %d\n", data.x, data.y, clientfd);

            xSend = htonl(data.x);
            ySend = htonl(data.y);

            send(clientfd, &xSend, sizeof(uint32_t), 0);
            send(clientfd, &ySend, sizeof(uint32_t), 0);
        }
        else if(strcmp(buffer, END) == 0) {
            break;
        }
        else {
            printf("%s\n", buffer);
            continue;
        }
    }

    printf("Stopping communication with %d\n", clientfd);
    close(clientfd);
}

int random_int() {
    int num;

    num = rand() % 2000;

    num -= 1000;

    return num;
}
