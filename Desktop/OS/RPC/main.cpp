#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int calculate(const std::string& op, int a, int b) {
    if (op == "ADD") return a + b;
    if (op == "SUB") return a - b;
    if (op == "MUL") return a * b;
    if (op == "DIV" && b != 0) return a / b;
    return 0;
}

void start_server() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    listen(server_fd, 3);
    std::cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);
        if (client_socket < 0) continue;

        char buffer[BUFFER_SIZE] = {0};
        read(client_socket, buffer, BUFFER_SIZE);
        std::string op;
        int a, b;
        sscanf(buffer, "%s %d %d", &op[0], &a, &b);
        int result = calculate(op, a, b);
        std::string response = std::to_string(result);
        send(client_socket, response.c_str(), response.size(), 0);
        close(client_socket);
    }
}

void start_client(const std::string& message) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    send(sock, message.c_str(), message.size(), 0);
    char buffer[BUFFER_SIZE] = {0};
    read(sock, buffer, BUFFER_SIZE);
    std::cout << "Result: " << buffer << "\n";
    close(sock);
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        start_server();
    } else {
        std::string command;
        for (int i = 1; i < argc; ++i) command += std::string(argv[i]) + " ";
        start_client(command);
    }
    return 0;
}
