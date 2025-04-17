#include "socket.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1000

void handleConnection(int* sock_fd_ptr) {
    // dereference pointer and get the socket file descriptor, then free up the
    // allocated memory
    int sock_fd = *((int*)sock_fd_ptr);
    free(sock_fd_ptr);

    // create the buffer to store the recieved data
    char buffer[MAX_BUFFER_SIZE];
    size_t bytes_read_from_client;

    // Handle the connection
    printf("Handling connection on socket %d\n", sock_fd);

    // loop from slides
    while (1) {
        // if there is an input string read, write it, else break the loop
        bytes_read_from_client = recv(sock_fd, buffer, sizeof(buffer), 0);
        if (bytes_read_from_client > 0)
            write(sock_fd, buffer, bytes_read_from_client);
        else
            break;
    }

    // close the socket
    printf("Closing socket %d\n", sock_fd);
    close(sock_fd);
    return;  // exit
}
