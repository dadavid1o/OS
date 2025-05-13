#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "colorprint.hpp"
const int BUFFER_SIZE = 1024;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./client <server_ip> <port>\n";
        return 1;
    }

   
    std::vector<std::string> successPatterns;
    successPatterns.push_back("done");
    successPatterns.push_back("success");
    successPatterns.push_back("ok");
    successPatterns.push_back("completed");
    
    std::vector<std::string> failPatterns;
    failPatterns.push_back("error");
    failPatterns.push_back("fail");
    failPatterns.push_back("oops");
    failPatterns.push_back("invalid");

  
    return 0;
}
