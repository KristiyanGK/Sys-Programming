#include <stdio.h> 
#include "server.h"

#define DEFAULT_PORT 10000

/*
    cmd arguments: 
    [1] port - number or 'default' for DEFAULT_PORT
*/
int main(int argc, char *argv[]) {
    int serverfd, maxCon;
    Status result;

    printf("Creating server...\n");

    result = create_server(DEFAULT_PORT, &serverfd);

    if (result < 0) {
        printf("status: %d\n", result);
    }

    maxCon = 5;

    printf("Starting server...\n");

    result = start_listening(serverfd, maxCon);

    if(result < 0) {
        printf("status: %d\n", result);
    }    
    
    return 0;
}
