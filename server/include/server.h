#ifndef SERVER_HEADER
#define SERVER_HEADER

// types

#define BUFF_SIZE 4
#define END "end"
#define GET "get"

typedef enum Status_Enum {
    E_SUCCESS = 1,
    E_INVALID_PORT = -1,
    E_FAILED_SOCKET_CREATION = -2,
    E_FAILED_BINDING = -3,
    E_FAILED_LISTENING = -4,
    E_FAILED_SERVER_ACCEPT = -5
} Status;

// functions
int create_server(int port, int* serverfd);
int start_listening(int serverfd, int maxConn);

#endif