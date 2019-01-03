#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

time_t start;
std::mutex mtx;
int count = 0;
// =0 - 绳索空闲
// >0 - 向东爬行的数量
// <0 - 向西爬行的数量

int east_count = 0;
int west_count = 0;

void print_count(const char* extra = "") {
	std::cout << "[" << std::setw(2) << time(nullptr) - start << "] count = ";
	std::cout << std::setw(2) << count;
	std::cout << " " << east_count << " " << west_count;
	std::cout << " " << extra << std::endl;
}

void baboon_nohunger(int direction) {
	if (direction > 0)
		west_count ++;
	else
		east_count ++;

	{
		// 尝试使用绳索
		std::unique_lock<std::mutex> lck(mtx);
		while (count != 0 && ((direction > 0 && east_count > 0)
						|| (direction < 0 && west_count > 0))) {
			lck.unlock();
			std::this_thread::yield();
			lck.lock();
		}
		count += direction;
		print_count();
	}

	sleep(5); // 模拟完全通过需要的时间

	{
		// 已完全通过
		std::unique_lock<std::mutex> lck(mtx);
		count -= direction;
		print_count("<finish>");
	}

	if (direction > 0)
		west_count --;
	else
		east_count --;
}

int main() {
	srand(time(nullptr));
	std::vector<std::thread> list;
	start = time(nullptr);
	while (list.size() < 30) {
		list.push_back(std::thread(baboon_nohunger, rand() % 2 == 0 ? 1 : -1));
		sleep(1); // 每秒新增一个非洲狒狒，行走方向随机
	}
	for (int i = 0; i < list.size(); ++i)
		list[i].join();
}