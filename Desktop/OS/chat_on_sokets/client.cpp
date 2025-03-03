#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return 1;
    }

    std::cout << "Connected to server!" << std::endl;

    char buffer[BUFFER_SIZE];
    while (true) {
        std::cout << "Enter message: ";
        std::cin.getline(buffer, BUFFER_SIZE);

        send(client_socket, buffer, strlen(buffer), 0);

        // Получаем сообщение от сервера
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            std::cout << "Server disconnected or error occurred!" << std::endl;
            break;
        }
        buffer[bytes_received] = '\0';
        std::cout << "Server: " << buffer << std::endl;
    }

    close(client_socket);
    return 0;
}
