#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main() {
	mkfifo("./fifo.sock", 0644);
	if (fork() == 0) {
		int pipe_fd = open("./fifo.sock", O_RDONLY /*| O_NONBLOCK*/);
		if (pipe_fd == -1) {
			std::cerr << "can not open fifo file." << std::endl;
			exit(-1);
		}
		char buffer[101];
		while (true) {
			int count = read(pipe_fd, buffer, 100);
			buffer[count] = 0;
			std::cout << "read " << count << " bytes: " << buffer << std::endl;
		}
	} else {
		int pipe_fd = open("./fifo.sock", O_WRONLY /*| O_NONBLOCK*/);
		if (pipe_fd == -1) {
			std::cerr << "can not open fifo file." << std::endl;
			exit(-1);
		}
		char buffer[100] = "hello world!";
		int length = strlen(buffer);
		while (true) {
			int count = write(pipe_fd, buffer, length);
			std::cout << "send " << count << " bytes: " << buffer << std::endl;
			sleep(1);
		}
	}
}