#ifndef QUEUE_HEADER
#define QUEUE_HEADER

typedef struct node {
    struct node* next;
    int *client_socket;
} node_t;

void enqueue(int *client_socket);
int* dequeue();

#endif