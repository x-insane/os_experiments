#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <memory.h>

bool check(int n, int m, const bool* finish_src,const int (*allocation_src)[1000],
		const int (*need_src)[1000], const int* available_src, bool print = false) {
	bool* finish = new bool[m];
	auto allocation = new int[m][1000];
	auto need = new int[m][1000];
	int* available = new int[n];
	memcpy(finish, finish_src, sizeof(int)*m);
	memcpy(allocation, allocation_src, sizeof(int)*m*1000);
	memcpy(need, need_src, sizeof(int)*m*1000);
	memcpy(available, available_src, sizeof(int)*n);

	if (print) {
		std::cout << "available:" << std::endl;
		for (int j = 0; j < n; j++)
			std::cout << available[j] << " ";
		std::cout << std::endl << "allocation:" << std::endl;
		for (int j = 0; j < m; j++) {
			for (int k = 0; k < n; k++)
				std::cout << allocation[j][k] << " ";
			std::cout << std::endl;
		}
		std::cout << "need:" << std::endl;
		for (int j = 0; j < m; j++) {
			for (int k = 0; k < n; k++)
				std::cout << need[j][k] << " ";
			std::cout << std::endl;
		}
	}

	bool loop = true;
	while (loop) {
		loop = false;
		for (int j = 0; j < m; ++j) {
			if (finish[j])
				continue;
			bool enough = true;
			for (int k = 0; k < n; ++k) {
				if (need[j][k] > available[k]) {
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

	delete[] need;
	delete[] allocation;
	delete[] finish;
	delete[] available;

	return success;
}

int main() {
	freopen("banker.input", "r", stdin);
	freopen("banker.output", "w", stdout);
	int count; // 测试数据组数
	std::cin >> count;
	for (int i = 0; i < count; ++i) {
		int n = 0, m = 0; // n类资源，m个进程
		std::cin >> n >> m;
		int* available = new int[n];
		for (int j = 0; j < n; ++j)
			std::cin >> available[j]; // 读取当前空闲资源
		auto allocation = new int[m][1000];
		memset(allocation, 0, sizeof(int)*m*1000);
		auto need = new int[m][1000]; // 假设资源数量不超过1000

		for (int j = 0; j < m; ++j) {
			for (int k = 0; k < n; ++k)
				std::cin >> need[j][k]; // 读取每个进程需要的资源数量
		}

		int p, r;
		bool* finish = new bool[m] { false };
		std::queue<int>* delay_queue = new std::queue<int>[m];
		while (std::cin >> p >> r) {
			if (p == 0)
				break;
			p--; r--;
			if (finish[p]) {
				std::cout << "进程" << p+1 << "请求资源" << r+1 << "：忽略（该进程已完成任务）" << std::endl;
				continue;
			}

			// 检查等待队列中的元素
			bool continue_flag = false;
			while (delay_queue[p].size()) {
				int rr = delay_queue[p].front();
				available[rr] --;
				allocation[p][rr] ++;
				need[p][rr] --;
				if (check(n, m, finish, allocation, need, available)) {
					delay_queue[p].pop();
					std::cout << "进程" << p+1 << "获得资源" << rr+1 << std::endl;
				} else {
					// 恢复状态
					available[rr] ++;
					allocation[p][rr] --;
					need[p][rr] ++;
					continue_flag = true;
					break;
				}
			}
			if (continue_flag) {
				std::cout << "进程" << p+1 << "请求资源" << r+1 << "：推迟" << std::endl;
				delay_queue[p].push(r);
				continue;
			}

			available[r] --;
			allocation[p][r] ++;
			need[p][r] --;
			if (check(n, m, finish, allocation, need, available)) {
				std::cout << "进程" << p+1 << "请求资源" << r+1 << "：通过" << std::endl;
				bool finish_p = true;
				for (int j = 0; j < n; ++j) {
					if (need[p][j]) {
						finish_p = false;
						break;
					}
				}
				if (finish_p) {
					// 任务已完成，释放所有资源
					std::cout << "进程" << p+1 << "任务已完成" << std::endl;
					for (int j = 0; j < n; ++j)
						available[j] += allocation[p][j];
					finish[p] = true;
				}
			} else {
				std::cout << "进程" << p+1 << "请求资源" << r+1 << "：推迟" << std::endl;
				delay_queue[p].push(r);
				// 恢复状态
				available[r] ++;
				allocation[p][r] --;
				need[p][r] ++;
			}
		}

		// 调度剩余的等待队列
		bool finish_all = true;
		while (true) {
			finish_all = true;
			bool impossible = true;
			for (p = 0; p < m; ++p) {
				if (delay_queue[p].size()) {
					int rr = delay_queue[p].front();
					available[rr] --;
					allocation[p][rr] ++;
					need[p][rr] --;
					if (check(n, m, finish, allocation, need, available)) {
						delay_queue[p].pop();
						std::cout << "进程" << p+1 << "获得资源" << rr+1 << std::endl;
						impossible = false; // 只要成功获取一次资源，就还有希望
						bool finish_p = true;
						for (int k = 0; k < n; ++k) {
							if (need[p][k]) {
								finish_p = false;
								break;
							}
						}
						if (finish_p) {
							// 任务已完成，释放所有资源
							std::cout << "进程" << p+1 << "任务已完成" << std::endl;
							for (int k = 0; k < n; ++k)
								available[k] += allocation[p][k];
							finish[p] = true;
						}
						if (delay_queue[p].size())
							finish_all = false;
					} else {
						// 恢复状态
						available[rr] ++;
						allocation[p][rr] --;
						need[p][rr] ++;
						finish_all = false;
					}
				}
			}
			if (impossible)
				break;
			if (finish_all)
				break;
		}
		if (finish_all)
			std::cout << "所有任务均已完成" << std::endl;
		else {
			if (!check(n, m, finish, allocation, need, available, true))
				std::cout << "check failed." << std::endl;
			std::cout << "无法完成所有任务" << std::endl;
			std::cout << "available:" << std::endl;
			for (int j = 0; j < n; j++)
				std::cout << available[j] << " ";
			std::cout << std::endl << "allocation:" << std::endl;
			for (int j = 0; j < m; j++) {
				for (int k = 0; k < n; k++)
					std::cout << allocation[j][k] << " ";
				std::cout << std::endl;
			}
			std::cout << "need:" << std::endl;
			for (int j = 0; j < m; j++) {
				for (int k = 0; k < n; k++)
					std::cout << need[j][k] << " ";
				std::cout << std::endl;
			}
			std::cout << "delay queue:" << std::endl;
			for (int j = 0; j < m; j++) {
				while (delay_queue[j].size()) {
					std::cout << delay_queue[j].front() << " ";
					delay_queue[j].pop();
				}
				std::cout << std::endl;
			}
		}
		std::cout << std::endl;

		delete[] delay_queue;
		delete[] finish;
		delete[] need;
		delete[] allocation;
		delete[] available;
	}
}