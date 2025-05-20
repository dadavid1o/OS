#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <string>
#include "Bank.h"

const int PORT = 12345;
const int BUFFER_SIZE = 1024;

int main() {
    int fd = shm_open("/bank", O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }

    void* addr = mmap(nullptr, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    Bank* bank = static_cast<Bank*>(addr);

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_sock);
        return 1;
    }

    if (listen(server_sock, 5) < 0) {
        perror("listen");
        close(server_sock);
        return 1;
    }

    std::cout << "Bank server listening on port " << PORT << "...\n";

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_len);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }

        std::cout << "Client connected\n";
        char buffer[BUFFER_SIZE];

        while (true) {
            ssize_t bytes_received = recv(client_sock, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_received <= 0) {
                std::cout << "Client disconnected\n";
                break;
            }

            buffer[bytes_received] = '\0';
            std::string cmd(buffer);

            std::ostringstream iss;
            if (!process_command(bank, cmd)) {
                iss << "[Server] Command failed or exit requested.\n";
            } else {
                iss << "[Server] Command executed successfully.\n";
            }

            std::string response = iss.str();
            send(client_sock, response.c_str(), response.size(), 0);

            if (cmd == "exit") break;
        }

        close(client_sock);
    }

    munmap(bank, sb.st_size);
    close(fd);
    close(server_sock);
    return 0;
}
