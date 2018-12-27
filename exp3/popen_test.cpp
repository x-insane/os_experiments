#include <iostream>
#include <unistd.h>
#include <string.h>

int main() {
	FILE *write_fp;
	char buffer[100] = "hello world!\nhello world!\nhello world!\nhello world!\n";
	write_fp = popen("grep hello", "w"); // 将数据输出到目标程序的标准输入流
	if (write_fp != NULL) {
		fwrite(buffer, sizeof(char), strlen(buffer), write_fp);
		pclose(write_fp);
	}
}