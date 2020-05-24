#include <stdio.h> 
#include <stdlib.h>
#include "server.h"

/*
    cmd arguments: 
    [1] port - number
*/
int main(int argc, char *argv[]) {
    int serverfd, maxCon;
    Status result;

    if (argc != 2)
    {
        printf("Invalid args count!\n");
        return 0;
    }
    

    printf("Creating server...\n");

    result = create_server(atoi(argv[1]), &serverfd);

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
