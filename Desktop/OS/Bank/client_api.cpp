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
    if (argc != 1) {
        std::cerr << "Usage: " << argv[0] << "\n";
        return 1;
    }

    int fd = shm_open("/bank", O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open failed");
        return 1;
    }

    struct stat buff;
    if (fstat(fd, &buff) == -1) { //считываем размер файла разделяемой памяти по дескриптору
        perror("fstat failed");
        close(fd);
        return 1;
    }
    // отображаем память в адресное пространство 
    void* addr = mmap(nullptr, buff.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    Bank* bank = static_cast<Bank*>(addr); // преобразуем тип void*

    std::cout << "[SUCCESS] Bank memory mapped. Total size: " << buff.st_size << " bytes.\n";

    std::string cmd;
   
    while (std::cout << "> ", std::getline(std::cin, cmd)) {
        std::ostringstream output; // поток строк где пишется ответ (просто сохраняем все в строку)
        bool keep_going = process_command(bank, cmd, output); // команда
        std::cout << output.str(); // получаем аут и печатаем...

        if (!keep_going) break; // если короч exit то выходим bistro
    }
    // освобаждаем память
    munmap(bank, buff.st_size);
    close(fd);
    return 0;

}
