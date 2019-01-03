#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <chrono>

int main(int argc, const char **argv) {
	auto start = std::chrono::high_resolution_clock::now();
	if (argc > 1) {
		// 执行单个命令
		std::string command;
		for (int i = 1; i < argc; ++i) {
			command += argv[i];
			command += " ";
		}
		system(command.c_str());
	} else {
		// 进入交互式命令模式
		while (true) {
			// 输出命令提示符
			std::cout << getcwd(nullptr, 0) << "$ ";
			std::string command;
			getline(std::cin, command);
			// 去掉首尾空格
			command.erase(0, command.find_first_not_of(" "));
			command.erase(command.find_last_not_of(" ") + 1);
			if (command == "exit")
				break;
			if (command.find("cd ") == 0) {
				if (command.size() < 4)
					std::cout << "usage: cd <directory>" << std::endl;
				else {
					const char* path = command.substr(3)
										.erase(0, command.find_first_not_of(" "))
										.c_str();
					if (chdir(path)) {
						switch (errno) {
							case EACCES:
								std::cout << "can not cd " << path << ": Permission denied." << std::endl;
								break;
							case ENOENT:
								std::cout << "can not cd " << path << ": not found." << std::endl;
								break;
							case ENOTDIR:
								std::cout << "can not cd " << path << ": not a directory." << std::endl;
								break;
							default:
								std::cout << "can not cd " << path << "." << std::endl;
								break;
						}
					}	
				}
			}
			else
				system(command.c_str());
		}
	}
	auto finish = std::chrono::high_resolution_clock::now();
	// 统计信息
	struct rusage usage_info;
	int err = getrusage(RUSAGE_CHILDREN, &usage_info);
	std::cout << "---------------------------" << std::endl;
	double utime = usage_info.ru_utime.tv_sec + usage_info.ru_utime.tv_usec / 1000000.0;
	double stime = usage_info.ru_stime.tv_sec + usage_info.ru_stime.tv_usec / 1000000.0;
	std::cout << "用户态执行时间: " << utime << " s" << std::endl;
	std::cout << "内核态执行时间: " << stime << " s" << std::endl;
	std::cout << "缺页次数:       " << usage_info.ru_majflt << std::endl;
	std::cout << "CPU切换次数:    " << usage_info.ru_nvcsw << std::endl;
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "程序的运行时间: " << elapsed.count() << " s" << std::endl;
}