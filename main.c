#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket.h"

#define LISTEN_BACKLOG 5
#define DEFAULT_PORT 8080

int main(int argc, char* argv[]) {
    int port = DEFAULT_PORT;

    // using the int to act as a T/F bool with 0 (false) and 1 (true). 
    // int is print_to_terminal from socket.h

    // check for -p argument in command line (./exe -p 'portnum') should be the
    // format, check every spot of the command line argument vector

    // check for -v value in the argument vector to see if we print to terminal
    // or not

    for (int ix = 0; ix < argc; ix++) {
        if (strcmp(argv[ix], "-p") == 0 && (ix + 1) < argc) {
            sscanf(argv[ix + 1], "%d", &port);
        }
        if (strcmp(argv[ix], "-v") == 0 && (ix + 1) < argc) {
            print_to_terminal = 1;
            printf("Printing to terminal\n");
        }
    }

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in socket_address;
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family      = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port        = htons(port);

    printf("Binding to port %d\n", port);

    int returnval;

    returnval = bind(socket_fd, (struct sockaddr*)&socket_address,
                     sizeof(socket_address));
    if (returnval < 0) {
        perror("bind");
        return 1;
    }

    returnval = listen(socket_fd, LISTEN_BACKLOG);
    if (returnval < 0) {
        perror("listen");
        return 1;
    }
    printf("Echo server is listening on port %d...\n", port);

    // (from slides)
    struct sockaddr_in client_address;

    while (1) {
        pthread_t thread;

        socklen_t client_address_len = sizeof(client_address);

        int* client_fd_buf           = malloc(sizeof(int));
        *client_fd_buf = accept(socket_fd, (struct sockaddr*)&client_address,
                                &client_address_len);

        printf("Accepted connection on %d\n", *client_fd_buf);

        pthread_create(&thread, NULL, (void* (*)(void*))incomingClientConnection,
                       (void*)client_fd_buf);

        pthread_detach(thread);
    }

    return 0;
}
