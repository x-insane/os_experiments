#include <iostream>
#include <unistd.h>
#include <string.h>

int main() {
	int pipes[2];
	if (pipe(pipes)) {
		std::cerr << "can not create anonymous pipe." << std::endl;
		exit(-1);
	}
	if (fork() == 0) {
		char buffer[101];
		while (true) {
			int count = read(pipes[0], buffer, 100);
			buffer[count] = 0;
			std::cout << "read " << count << " bytes: " << buffer << std::endl;
		}
	} else {
		char buffer[100] = "hello world!";
		int length = strlen(buffer);
		while (true) {
			int count = write(pipes[1], buffer, length);
			std::cout << "send " << count << " bytes: " << buffer << std::endl;
			sleep(1);
		}
	}
}