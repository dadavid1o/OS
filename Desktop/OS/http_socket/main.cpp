#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

#define REQUEST "GET / HTTP/1.1\r\nHost: httpforefer.com\r\nConectetion: close\r\n\r\n"
int main() {

    // server address
    struct sockaddr_in server_address;

    // create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket creation error");
        exit(errno);
    }

    // fill the server ip and port address
    server_address.sin_addr.s_addr = inet_addr("146.190.62.39");
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(80);

    // connect
    int connected = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    if (connected == -1) {
        perror("connection failed");
        exit(errno);
    }

    std::string mess = REQUEST;
    // send message to server
    int sent = send(client_socket, mess.c_str(), mess.size(), 0);
    if (sent == -1) {
        perror("send failed");
        exit(errno);
    }

    char buffer[3001];
    int rs;
    while((rs = recv(client_socket, buffer, 3000,0)) > 0) {
        std::cout << "Got messege:\n";
        buffer[rs] = '\0';
        std::cout << buffer << "\n";
    }
    if(rs == -1) {
        perror("client socket connection error");
        close(client_socket);
    }
    if(rs == 0) {
        std::cout << "empty massage\n";
    }
    close(client_socket);
    return 0;
}

