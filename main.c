#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket.h"

#define LISTEN_BACKLOG 5
#define PORT 8080



int main(int argc, char* argv[]) {

    // Create listening socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // check if socket was created successfully
    if (socket_fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in socket_address;
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family      = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port        = htons(PORT);

    printf("Binding to port %d\n", PORT);

    int returnval;

    returnval = bind(socket_fd, (struct sockaddr*)&socket_address,
                     sizeof(socket_address));
    if (returnval < 0) {
        perror("bind");
        return 1;
    }

    // listen for incoming connection
    returnval = listen(socket_fd, LISTEN_BACKLOG);
    if (returnval < 0) {
        perror("listen");
        return 1;
    }
    printf("Echo server is listening on port %d...\n", PORT);

    // (from slides)
    struct sockaddr_in client_address;

    while (1) {
        pthread_t thread;

        socklen_t client_address_len = sizeof(client_address);

        int* client_fd_buf           = malloc(sizeof(int));
        *client_fd_buf = accept(socket_fd, (struct sockaddr*)&client_address,
                                &client_address_len);

        printf("Accepted connection on %d\n", *client_fd_buf);

        pthread_create(&thread, NULL, (void* (*)(void*))handleConnection, (void*)client_fd_buf);

        pthread_detach(thread);

    }

    return 0;
}
