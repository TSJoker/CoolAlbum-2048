#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = open("111.c", O_RDWR); //以可读可写的方式打开open.c 这个文件
	if(fd == -1) //判断是不是打开失败了
	{
		perror(""); //刷新os提供的错误信息
		return -1;
	}
	
	printf("open file success\n"); //打印信息意思意思
	
	char buf[10] = "qweqweqw";  //buf[0] = 'q' buf[1]= 'w'  buf[2] = 'e' ......
	write(fd, &buf[0], 8);
	write(fd, &buf[0], 8);
	write(fd, &buf[0], 8);
	
	
}











