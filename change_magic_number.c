// 用于改变ext2文件系统的magic number
// 将会查看指定文件（分区也可以看做文件）的0x438位置
// 如果该位置是0xef53就改为0x6666

#include <stdio.h>

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("usage: %s %s\n", argv[0], "filename");
		return 1;
	}
	FILE* file = fopen(argv[1], "r+b");
	if (file == NULL)
	{
		printf("can not to open file: %s\n", argv[1]);
		return 1;
	}
	fseek(file, 0x438, SEEK_SET);
	unsigned short magic = 0;
	fread(&magic, 2, 1, file);
	if (0xef53 == magic)
	{
		fseek(file, 0x438, SEEK_SET);
		magic = 0x6666;
		fwrite(&magic, 2, 1, file);
		printf("magic number changed 0x%x => 0x%x\n", 0xef53, magic);
	}
	else
		printf("magic number = 0x%x.\nnot a ext2 format. magic number not changed.\n", magic);
	fclose(file);
	return 0;
}