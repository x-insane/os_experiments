#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

time_t start;
std::mutex mtx;
std::condition_variable east, west;
int count = 0;
// =0 - 绳索空闲
// >0 - 向东爬行的数量
// <0 - 向西爬行的数量

void print_count(const char* extra = "") {
	std::cout << "[" << std::setw(2) << time(nullptr) - start << "] count = ";
	std::cout << std::setw(2) << count << " " << extra << std::endl;
}

void baboon(int direction) {
	{
		// 尝试使用绳索
		std::unique_lock<std::mutex> lck(mtx);
		while (count != 0 && ((count > 0 && direction < 0) ||
							(count < 0 && direction > 0))) {
			// 不同向，等待
			if (direction < 0)
				west.wait(lck);
			else
				east.wait(lck);
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
		if (count == 0) {
			if (direction < 0)
				east.notify_all();
			else
				west.notify_all();
		}
	}
}

int main() {
	srand(time(nullptr));
	std::vector<std::thread> list;
	start = time(nullptr);
	while (list.size() < 30) {
		list.push_back(std::thread(baboon, rand() % 2 == 0 ? 1 : -1));
		sleep(1); // 每秒新增一个非洲狒狒，行走方向随机
	}
	for (int i = 0; i < list.size(); ++i)
		list[i].join();
}