#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>

#include "colorprint.hpp"

const int BUFFER_SIZE = 1024;

int main(int argc, char* argv[]) {
    
    const char* SERVER_IP = argv[1];
    const int PORT = (std::atoi(argv[2]));
    
    if (argc != 3) {
        std::cerr << "Usage: ./client <server_ip> <port>\n";
        return 1;
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid IP address\n";
        return 1;
    }

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }
    std::vector<std::string> successPatterns;
    successPatterns.push_back("done");
    successPatterns.push_back("successfully");
    successPatterns.push_back("ok");
    successPatterns.push_back("completed");
    
    std::vector<std::string> failPatterns;
    failPatterns.push_back("error");
    failPatterns.push_back("failed");
    failPatterns.push_back("oops");
    failPatterns.push_back("invalid");
    
    Painter painter(std::cout, successPatterns, failPatterns);

    std::string cmd;
    char buffer[BUFFER_SIZE];
    
    while (std::cout << "> ", std::getline(std::cin, cmd)) {

        if (send(sock, cmd.c_str(), cmd.size(), 0) == -1) {
            perror("send");
            break;
        }

        if(cmd == "exit") {
            break;
        }

        ssize_t bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("recv");
            break;
        }
        buffer[bytes_received] = '\0';
        painter.printColoredLine(buffer);
    }
    close(sock);
    return 0;
}
