#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main() {
	if (fork() == 0) {
		alarm(5);
		signal(SIGALRM, [](int i) {
			std::cout << "send a signal SIGALRM to parent." << std::endl;
			kill(getppid(), SIGALRM);
			exit(0);
		});
		signal(SIGINT, [](int i) {
			
		});
		signal(SIGTERM, [](int i) {
			
		});
		while (true)
			pause();
	}
	signal(SIGINT, [](int i) {
		std::cout << std::endl << "catch a signal: SIGINT" << std::endl;
		// exit(-1);
	});
	signal(SIGTERM, [](int i) {
		std::cout << "catch a signal: SIGTERM" << std::endl;
		// exit(-1);
	});
	signal(SIGALRM, [](int i) {
		std::cout << "catch a signal: SIGALRM\nexit." << std::endl;
		exit(0);
	});
	while (true)
		pause();
}