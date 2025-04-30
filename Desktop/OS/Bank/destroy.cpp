#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {

	int fd = shm_open("/bank", O_RDONLY, 0666);
	if(fd == -1) {
		perror("shm open error");
		return 1;
	}
	close(fd);
	
	if(shm_unlink("/bank") == -1) {
		perror("shm_unlink");
		return 1;
	}
	std::cout << "OKEY bro bank destroyed";
	return 0;
}
