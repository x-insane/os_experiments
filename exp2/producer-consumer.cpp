#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <thread>

const int SIZE = 100;
int queue[SIZE];
int head = 0;
int tail = 0;

void produce() {
	static int last_print_time = time(nullptr);
	if (time(nullptr) - last_print_time > 0) {
		last_print_time = time(nullptr);
		std::cout << ".";
		std::cout.flush();
	}
	while (tail + 1 % SIZE == head)
		usleep(100000); // 满
	queue[tail++] = rand();
	usleep(50000); // 累了，歇会儿
	tail %= SIZE;
}

void consume() {
	while (head == tail)
		usleep(100000); // 空
	int data = queue[head++];
	usleep(200000); // 累了，歇会儿
	head %= SIZE;
}

void producer() {
	while (true)
		produce();
}

void consumer() {
	while (true)
		consume();
}

int main() {
	srand(time(nullptr));
	std::vector<std::thread> threads;
	for (int i = 0; i < 10; ++i)
		threads.push_back(std::thread(consumer));
	for (int i = 0; i < 2; ++i)
		threads.push_back(std::thread(producer));
	for (int i = 0; i < threads.size(); i++)
		threads[i].join();
	return 0;
}