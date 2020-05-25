#include <stdio.h> 
#include <stdlib.h>
#include "server.h"

/*
    cmd arguments: 
    [1] port - number
    [2] time(secs) - number
    [3] thread_pool_size - number
*/
int main(int argc, char *argv[]) {
    int serverfd, port, time, thread_pool_size;
    Status result;

    if (argc != 4)
    {
        printf("Invalid args count!\n");
        return 0;
    }

    printf("Creating server...\n");

    port = atoi(argv[1]);

    result = create_server(atoi(argv[1]), &serverfd);

    if (result < 0) {
        printf("status: %d\n", result);
    }

    printf("Starting server...\n");

    time = atoi(argv[2]);
    thread_pool_size = atoi(argv[3]);

    result = start_listening(serverfd, time, thread_pool_size);

    if(result < 0) {
        printf("status: %d\n", result);
    }    
    
    return 0;
}
