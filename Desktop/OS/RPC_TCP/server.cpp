#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int client_fd;
    char buffer[BUFFER_SIZE];
} ThreadData;

void* handle_client(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    printf("Client %d sent: %s\n", data->client_fd, data->buffer);

    send(data->client_fd, data->buffer, strlen(data->buffer), 0);

    delete data; 
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    struct pollfd fds[MAX_CLIENTS + 1];
    int nfds = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, MAX_CLIENTS);

    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    std::cout << "Server started on port " << PORT << std::endl;

    while (true) {
        int ret = poll(fds, nfds, -1);
        if (ret < 0) {
            perror("poll error");
            break;
        }

        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_fd) {
                    struct sockaddr_in client_addr;
                    socklen_t addr_len = sizeof(client_addr);
                    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);

                    if (client_fd < 0) {
                        perror("accept error");
                        continue;
                    }

                    fds[nfds].fd = client_fd;
                    fds[nfds].events = POLLIN;
                    nfds++;

                    std::cout << "New client connected: " << client_fd << std::endl;
                } else {
                    char buffer[BUFFER_SIZE] = {0};
                    ssize_t bytes_read = recv(fds[i].fd, buffer, BUFFER_SIZE - 1, 0); // Оставляем место для '\0'

                    if (bytes_read > 0) {
                        buffer[bytes_read] = '\0'; 

                        pthread_t thread;
                        ThreadData* data = new ThreadData;
                        data->client_fd = fds[i].fd;
                        memcpy(data->buffer, buffer, bytes_read + 1); 

                        pthread_create(&thread, NULL, handle_client, data);
                        pthread_detach(thread);
                    } else if (bytes_read == 0) {
                        std::cout << "Client " << fds[i].fd << " disconnected" << std::endl;
                        close(fds[i].fd);
                        memmove(&fds[i], &fds[i + 1], (nfds - i - 1) * sizeof(struct pollfd));
                        nfds--;
                        i--;
                    } else {
                        perror("recv error");
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}