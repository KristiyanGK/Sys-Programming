#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include "client.h"

int setup_connection(const char *address, const int port) {
    int sockfd;
    struct sockaddr_in serveraddr = {0}, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { 
        printf("Socket creation failed...\n");
        printf("%s\n", strerror(errno));
        exit(0); 
    } 
    else {
        printf("Socket successfully created...\n"); 
    }

    // assign IP, PORT 
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = inet_addr(address);
    serveraddr.sin_port = htons(port);

    // connect the client socket to server socket 
    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) != 0) { 
        printf("Connection with the server failed...\n");
        printf("%s\n", strerror(errno));
        exit(0); 
    } 
    else {
        printf("Connected to the server...\n"); 
    }

    return sockfd;
}

void client_send(int sockfd, const void* buffer, size_t size) {
    send(sockfd, buffer, size, 0);
}

void client_recv(int sockfd, void* buffer, size_t size) {
    recv(sockfd, buffer, size, 0);
}

void client_close(int sockfd) {
    close(sockfd);
}
