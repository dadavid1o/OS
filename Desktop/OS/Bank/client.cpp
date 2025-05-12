#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <sys/mman.h>
#include <climits>
#include <unistd.h>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include "Bank.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_accounts>\n";
        return 1;
    }
    int fd = shm_open("/bank", O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open failed");
        return 1;
    }

    struct stat buff;
    if (fstat(fd, &buff) == -1) {
        perror("fstat failed");
        close(fd);
        return 1;
    }

    void* addr = mmap(nullptr, buff.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    Bank* bank = static_cast<Bank*>(addr);
    close(fd);

    std::cout << "[SUCCESS] Bank memory mapped. Total size: " << buff.st_size << " bytes.\n";
    return 0;
}
