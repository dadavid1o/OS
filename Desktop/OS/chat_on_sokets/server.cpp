#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <pthread.h>
#include <signal.h>

#define PORT 8888
#define BUFFER_SIZE 1024

// Список клиентов
std::vector<int> clients;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void* handle_client(void* arg);
void broadcast(const char* message, int sender);

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        return 1;
    }

    signal(SIGCHLD, SIG_IGN);

    std::cout << "Server is listening on port " << PORT << "..." << std::endl;

    while (true) {
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Защита списка клиентов с помощью мьютекса
        pthread_mutex_lock(&clients_mutex);
        clients.push_back(client_socket);
        pthread_mutex_unlock(&clients_mutex);

        std::cout << "New client connected!" << std::endl;

        // Создание нового потока для обработки клиента
        pthread_t thread_id;
        pthread_create(&thread_id, nullptr, handle_client, (void*)&client_socket);
    }

    close(server_socket);
    return 0;
}

// обработка клиента
void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    char buffer[BUFFER_SIZE];

    while (true) {
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            break;
        }
        buffer[bytes_received] = '\0';  // Завершаем строку


        broadcast(buffer, client_socket);
    }


    close(client_socket);

    pthread_mutex_lock(&clients_mutex);
    auto it = std::find(clients.begin(), clients.end(), client_socket);
    if (it != clients.end()) {
        clients.erase(it);
    }
    pthread_mutex_unlock(&clients_mutex);

    std::cout << "Client disconnected!" << std::endl;

    return nullptr;
}

// рассылка сообщений всем клиентам
void broadcast(const char* message, int sender) {
    pthread_mutex_lock(&clients_mutex);
    for (int client : clients) {
        if (client != sender) {
            send(client, message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}
