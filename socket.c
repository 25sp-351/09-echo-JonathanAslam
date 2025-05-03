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

int print_to_terminal = 0;

void incomingClientConnection(int* sock_fd_ptr) {
    // dereference pointer and get the socket file descriptor, then free up the
    // allocated memory
    int sock_fd = *((int*)sock_fd_ptr);
    free(sock_fd_ptr);

    char recieved_data_buffer[MAX_BUFFER_SIZE];
    size_t bytes_read_from_client;

    printf("Handling connection on socket %d\n", sock_fd);

    // loop from slides
    while (1) {
        // if there is an input string read, write it, else break the loop
        //FROM PROFESSOR OFFICE HOURS:
            // read until size of buffer-1 and then set the end of the buffer to a null terminated string
        bytes_read_from_client = read(sock_fd, recieved_data_buffer, sizeof(recieved_data_buffer)-1);
        recieved_data_buffer[bytes_read_from_client] = '\0';
        if (bytes_read_from_client > 0)
            if (print_to_terminal == 1){
                write(sock_fd, recieved_data_buffer, bytes_read_from_client);
            } else {
                continue;
            }
        else
            break;
    }

    printf("Closing socket %d\n", sock_fd);
    close(sock_fd);
    return;  
}