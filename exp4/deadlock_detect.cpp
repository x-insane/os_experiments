#include <iostream>
#include <cstdio>
#include <vector>

int main() {
	freopen("deadlock_detect.in", "r", stdin);
	int count; // 测试数据组数
	std::cin >> count;
	for (int i = 0; i < count; ++i) {
		int n = 0, m = 0; // n类资源，m个进程
		std::cin >> n >> m;
		int* available = new int[n];
		bool* finish = new bool[m] { false };
		for (int j = 0; j < n; ++j)
			std::cin >> available[j]; // 读取当前空闲资源
		auto allocation = new int[m][1000];
		auto request = new int[m][1000]; // 假设资源数量不超过1000

		for (int j = 0; j < m; ++j) {
			for (int k = 0; k < n; ++k)
				std::cin >> allocation[j][k]; // 读取每个进程的持有资源数量
			for (int k = 0; k < n; ++k)
				std::cin >> request[j][k]; // 读取每个进程正在请求资源数量
		}

		/****** Core ******/
		bool loop = true;
		std::vector<int> list;
		while (loop) {
			loop = false;
			for (int j = 0; j < m; ++j) {
				if (finish[j])
					continue;
				bool enough = true;
				for (int k = 0; k < n; ++k) {
					if (request[j][k] > available[k]) {
						enough = false;
						break;
					}
				}
				// 足够分配，分配并释放进程j所占资源
				if (enough) {
					for (int k = 0; k < n; ++k)
						available[k] += allocation[j][k];
					finish[j] = true;
					loop = true;
					list.push_back(j+1);
					break;
				}
			}
		}
		bool success = true;
		for (int j = 0; j < m; ++j) {
			if (!finish[j]) {
				success = false;
				break;
			}
		}
		if (success) {
			std::cout << "安全序列：";
			for (int j = 0; j < list.size(); ++j)
				std::cout << list[j] << " ";
			std::cout << std::endl;
		} else 
			std::cout << "存在死锁" << std::endl;
		/****** Core ******/

		delete[] request;
		delete[] allocation;
		delete[] finish;
		delete[] available;
	}
}