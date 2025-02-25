#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation error");
        exit(errno);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(8888);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind error");
        exit(errno);
    }

    if (listen(server_socket, 10) < 0) {
        perror("Listen error");
        exit(errno);
    }
    std::cout << "Waiting for connections...\n";

    while (true) {
        int client_socket;
        struct sockaddr_in client_address;
        unsigned int client_addr_len = sizeof(client_address);

        if ((client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_addr_len)) < 0) {
            perror("Accept error");
            exit(errno);
        }

        std::cout << "Connected with client at address: " << inet_ntoa(client_address.sin_addr) << "\n";

        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork error");
            exit(errno);
        }

        if (pid == 0) {  
            close(server_socket);  

            char buffer[1001];
            int rs = recv(client_socket, buffer, 1000, 0);
            if (rs == -1) {
                perror("Error receiving message");
                close(client_socket);
                exit(errno);
            }

            if (rs > 0) {
                buffer[rs] = '\0'; 
                std::cout << "Received message:\n" << buffer << "\n";
            }

            close(client_socket); 
            exit(0);  
        } else {  
            close(client_socket);  
        }
    }

    close(server_socket);
    return 0;
}
