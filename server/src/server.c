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
#include "queue.h"

// private functions declarations
int random_int();
void handle_connection(int clientfd);
void *worker(void *input);
void *update_coordinates(void *input);

// global variables
coordinates data = {0};
pthread_t *thread_pool;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

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

int start_listening(int serverfd, int time, int thread_pool_size) {
    int len, clientfd, i;
    struct sockaddr_in cli;
    pthread_t coordinates_thread;

    thread_pool = (pthread_t*) malloc(sizeof(pthread_t) * thread_pool_size);

    for (i = 0; i < thread_pool_size; i++) {
        pthread_create(&thread_pool[i], NULL, worker, NULL);
    }
    
    pthread_create(&coordinates_thread, NULL, update_coordinates, (void*)&time);

    if((listen(serverfd, MAX_CONN)) != 0) {
        perror("");
        return E_FAILED_LISTENING;
    }

    len = sizeof(cli);

    printf("Server is listening!\n");

    while(1) {
        clientfd = accept(serverfd, (struct sockaddr*)&cli, &len);

        if (clientfd < 0) {
            perror("");
            return E_FAILED_SERVER_ACCEPT;
        }

        printf("Received Request from %d\n", clientfd);

        int *pclientfd = (int*) malloc(sizeof(int));
        *pclientfd = clientfd;
        pthread_mutex_lock(&lock);
        enqueue(pclientfd);
        pthread_cond_signal(&condition_var);
        pthread_mutex_unlock(&lock);
    }

    return E_SUCCESS;
}

// private function definitions
void *worker(void *input) {
    while(1) {
        int *pclient;
        pthread_mutex_lock(&lock); 

        if ((pclient = dequeue()) == NULL) 
        {
            pthread_cond_wait(&condition_var, &lock);

            pclient = dequeue();
        }
        
        pthread_mutex_unlock(&lock);

        if (pclient != NULL)
        {
            handle_connection(*pclient);
            free(pclient);
        }
    }
}

void *update_coordinates(void *input) {
    int time = *(int*)input;
    while (1) {
        data.x = random_int();
        data.y = random_int();
        sleep(time);
    }
}

void handle_connection(int clientfd) {
    uint32_t xSend, ySend;
    char buffer[BUFF_SIZE] = {0};

    for(;;) {
        recv(clientfd, buffer, BUFF_SIZE, 0);

        if (strcmp(buffer, GET) == 0) {            
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
