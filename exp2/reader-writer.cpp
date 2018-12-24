#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <mutex>
#include <vector>
#include <thread>

int read_semaphore = 0;
bool write_waiting = false;
std::mutex read_lock;
std::mutex write_lock;

#define READ_COST_TIME    200000
#define WRITE_COST_TIME   400000
#define LOCK_FAIL_WAITING  50000

bool write_first = true;

void wait_rand_time() {
	// 打乱线程次序
	usleep(rand() % 500000 + 500000);
}

void reader() {
	wait_rand_time();
	while (true) {
		write_lock.lock();
		if (write_waiting) {
			write_lock.unlock();
			usleep(LOCK_FAIL_WAITING);
			continue;
		}
		break;
	}
	read_lock.lock();
	read_semaphore ++;
	read_lock.unlock();
	write_lock.unlock();
	std::cout << ".";
	std::cout.flush();
	usleep(READ_COST_TIME);
	read_lock.lock();
	read_semaphore --;
	read_lock.unlock();
}

void writer() {
	wait_rand_time();
	if (write_first) {
		while (true) {
			write_lock.lock();
			if (write_waiting) {
				write_lock.unlock();
				usleep(LOCK_FAIL_WAITING);
				continue;
			}
			write_waiting = true;
			write_lock.unlock();
			break;
		}
		while (true) {
			read_lock.lock();
			if (read_semaphore > 0) {
				read_lock.unlock();
				usleep(LOCK_FAIL_WAITING);
				continue;
			}
			read_lock.unlock();
			break;
		}
		std::cout << "+";
		std::cout.flush();
		usleep(WRITE_COST_TIME);
		write_lock.lock();
		write_waiting = false;
		write_lock.unlock();
	} else {
		while (true) {
			write_lock.lock();
			read_lock.lock();
			if (write_waiting || read_semaphore > 0) {
				read_lock.unlock();
				write_lock.unlock();
				usleep(LOCK_FAIL_WAITING);
				continue;
			}
			write_waiting = true;
			read_lock.unlock();
			write_lock.unlock();
			break;
		}
		std::cout << "+";
		std::cout.flush();
		usleep(WRITE_COST_TIME);
		write_lock.lock();
		write_waiting = false;
		write_lock.unlock();
	}
}

void test() {
	time_t start = time(nullptr);
	std::vector<std::thread> threads;
	int reader_count = 975;
	int writer_count = 25;
	for (int i = 0; i < reader_count; i++)
		threads.push_back(std::thread(reader));
	std::cout << "created " << reader_count << " read threads." << std::endl;
	for (int i = 0; i < writer_count; i++)
		threads.push_back(std::thread(writer));
	std::cout << "created " << writer_count << " write threads." << std::endl;
	for (int i = 0; i < threads.size(); i++)
		threads[i].join();
	std::cout << std::endl << "test cost " << time(nullptr) - start << " s." << std::endl;
}

int main() {
	srand(time(nullptr));
	test();
	write_first = false;
	test();
	return 0;
}