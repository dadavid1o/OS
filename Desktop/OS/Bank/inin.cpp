#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>

struct BankCell {
    int balance = 0;
    int min_balance = 0;
    int max_balance = 10000;
    bool frozen = false;
};

struct Bank {
    int size;
    BankCell cells[1]; // Гибкий массив

    Bank(int n) : size(n) {}
};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_accounts>\n";
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        std::cerr << "Invalid number of accounts (must be positive)\n";
        return 1;
    }

    int fd = shm_open("/bank", O_CREAT | O_EXCL | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open failed");
        return 1;
    }

    size_t size = sizeof(Bank) + (n - 1) * sizeof(BankCell);
    if (ftruncate(fd, size) == -1) {
        perror("ftruncate failed");
        close(fd);
        return 1;
    }

    void* addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    Bank* bank = new (addr) Bank(n);
    for (int i = 0; i < n; ++i) {
        new (&bank->cells[i]) BankCell();
    }

    close(fd);
    std::cout << "[SUCCESS] Bank initialized with " << n << " accounts\n";
    return 0;
}