#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main()
{
	int fd = open("/dev/fb0", O_RDWR); //以只读只写 的方式打开屏幕文件  /dev/fb0 
	if(fd == -1) //打开文件错误
	{
		perror(""); //把错误信息刷新出来
		return -1;
	}
	
	//映射这个文件的空间到我这个程序的内存空间中，使得我能够直接操作
	int *p = mmap(NULL, 800*480*4, PROT_READ | PROT_WRITE,MAP_SHARED,fd, 0);
	if(p == MAP_FAILED) //映射失败了
	{
		perror(""); //打印错误信息
		close(fd);//关闭文件
		return -1;//结束游戏
	}
	
	//p 指向了那个屏幕文件对应空间的开始位置
	// *p = 0xff;  //把第一个像素点的颜色改成蓝色
	// *(p+ 1) = 0xff; //把第二个像素点的颜色改成蓝色，指针加减，每+1 ，对应的地址挪动 sizeof(int) => 4个字节位置
	// 挪动4个字节，就到了下个像素点的位置
	// *(p + 800*i  + j)  => i表示行数   j表示列数  =》 *(p + 800*10 + 12) = 0xff; //让第10行第12列那个点变成蓝色
	// 此时行数和列数是从0开始数的
	int i,j,k,l;
	for(i = 0;i < 480;i++)
		for(j = 0;j < 800;j++)
		{
	 k = (i-240)*(i-240);
			l = (j-400)*(j-400);
			if((k + l) < 10000)
			*(p + 800*i + j) = 0xff0000; //不需要write屏幕直接变成对应的颜色，因为我现在就是直接修改文件的内容
      else		
      *(p + 800*i + j) = 0xffffff;				
		}
	

	close(fd);//文件不用了，一定要关闭，因为一个程序能打开的文件个数是有上限的  1024个 ，如果你总是打开文件，那么后面文件就不能打开了
	return 0;
}

