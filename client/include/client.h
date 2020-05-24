#ifndef CLIENT_HEADER
#define CLIENT_HEADER

#include <stdlib.h>

// types

#define BUFF_SIZE 5
#define END "end"
#define GET "get"

// functions
int setup_connection(const char *address, const int port);
void client_send(int sockfd, const void* buffer, size_t size);
void client_recv(int sockfd, void* buffer, size_t size);
void client_close(int sockfd);

#endif
