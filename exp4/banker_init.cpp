#include <iostream>
#include <cstdio>

int main() {
	freopen("banker.init", "r", stdin);
	freopen("banker.input", "w", stdout);
	srand(time(nullptr));
	int count;
	std::cin >> count;
	std::cout << count << std::endl << std::endl;
	for (int i = 0; i < count; ++i) {
		int n = 0, m = 0; // n类资源，m个进程
		std::cin >> n >> m;
		std::cout << n << " " << m << std::endl;
		int* res_count = new int[n];
		for (int j = 0; j < n; ++j) {
			std::cin >> res_count[j];
			std::cout << res_count[j];
			if (j != n-1)
				std::cout << " ";
		}
		std::cout << std::endl;
		auto need_res_count = new int[m][1000]; // 假设资源数量不超过1000
		for (int j = 0; j < m; ++j) {
			for (int k = 0; k < n; ++k) {
				std::cin >> need_res_count[j][k];
				std::cout << need_res_count[j][k];
				if (k != n-1)
					std::cout << " ";
			}
			std::cout << std::endl;
		}

		int* finish = new int[m] { false };
		while (true) {
			int p = rand() % m;
			if (finish[p])
				continue;
			int index = rand() % n;
			bool empty = true;
			for (int j = 0; j < n; ++j) {
				int k = (index + j) % n;
				if (need_res_count[p][k]) {
					empty = false;
					need_res_count[p][k] --;
					std::cout << p + 1 << " " << k + 1 << std::endl;
					break;
				}
			}
			finish[p] = empty;
			bool over = true;
			for (int j = 0; j < m; ++j) {
				if (!finish[j]) {
					over = false;
					break;
				}
			}
			if (over)
				break;
		}
		std::cout << 0 << " " << 0 << std::endl;

		std::cout << std::endl;
		delete[] finish;
		delete[] need_res_count;
		delete[] res_count;
	}
}