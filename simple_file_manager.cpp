#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void exec(std::string command) {
	// 去掉首尾空格
	command.erase(0, command.find_first_not_of(" "));
	command.erase(command.find_last_not_of(" ") + 1);
	if (command == "exit")
		exit(0);
	if (command.find("cd") == 0) {
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
	else if (command.find("creat") == 0) {
		if (command.size() < 7)
			std::cout << "usage: creat <filename>" << std::endl;
		else
			system(command.replace(0, 5, "touch").c_str());
	}
	else if (command.find("delete") == 0) {
		if (command.size() < 8)
			std::cout << "usage: delete <filename>" << std::endl;
		else
			system(command.replace(0, 6, "rm").c_str());
	}
	else if (command.find("mkdir") == 0) {
		if (command.size() < 7)
			std::cout << "usage: mkdir <dirname>" << std::endl;
		else
			system(command.c_str());
	}
	else if (command.find("rmdir") == 0) {
		if (command.size() < 7)
			std::cout << "usage: rmdir <dirname>" << std::endl;
		else
			system(command.replace(0, 5, "rm -r").c_str());
	}
	else if (command == "list")
		system("ls -al");
	else if (command.find("list") == 0) {
		if (command.size() < 6)
			std::cout << "usage: list [<dirname>]" << std::endl;
		else
			system(command.replace(0, 4, "ls -al").c_str());
	}
	else {
		std::cout << "unsupported command " << command.substr(0, command.find(" ")) << std::endl;
		std::cout << "supported command: cd/list/creat/delete/mkdir/rmdir/exit" << std::endl;
	}
}

int main(int argc, const char **argv) {
	if (argc > 1) {
		// 预执行单个命令
		std::string command;
		for (int i = 1; i < argc; ++i) {
			command += argv[i];
			command += " ";
		}
		exec(command);
	}
	// 进入交互式命令模式
	while (true) {
		// 输出命令提示符
		std::cout << getcwd(nullptr, 0) << "$ ";
		std::string command;
		getline(std::cin, command);
		exec(command);
	}
}