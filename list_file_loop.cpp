#include <iostream>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void list_file(std::string path, int indent = 0) {
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(path.c_str())) == nullptr) {
		std::cerr << "E: Cannot read path: " << path << std::endl;
		return;
	}
	while ((dirp = readdir(dp)) != nullptr) {
		if (strcmp(".", dirp->d_name) == 0)
			continue; // 忽略当前目录
		if (strcmp("..", dirp->d_name) == 0)
			continue; // 忽略上一级目录
		std::string file = path + "/" + dirp->d_name;
		struct stat file_stat;
		stat(file.c_str(), &file_stat);
		for (int i = 0; i < indent; ++i)
			std::cout << " ";
		std::cout << dirp->d_name << " " << std::endl;
		if (S_ISDIR(file_stat.st_mode))
			list_file(file, indent + 2);
	}
	closedir(dp);
}

int main(int argc, const char **argv) {
	if (argc > 1) {
		for (int i = 1; i < argc; ++i) {
			std::cout << argv[i] << std::endl;
			list_file(argv[i], 2);
		}
	} else
		list_file(".");
	return 0;
}