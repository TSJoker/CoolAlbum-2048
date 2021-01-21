//循环读取触摸屏的数据
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/input.h>

int main()
{
    // 打开触摸屏设备(/dev/input/event0)
    int fd_touch = open("/dev/input/event0",O_RDONLY);
    if (-1 == fd_touch)
    {
        //打开触摸屏设备失败
        perror("open fd_touch error");
        return -1;
    }
    
    // 创建一个结构体变量来接收读到的数据
    struct input_event ev;
	int x,y,x0,y0,x1,y1;
    // 循环读取触摸屏数据
    while (1)
    {
        read(fd_touch,&ev,sizeof(ev));
       
	if(EV_ABS == ev.type)
	{
		if(ABS_X == ev.code)
		{
		x=ev.value;
		}
		else if(ABS_Y == ev.code)
		{
		y=ev.value;
		}
	}
	else if((EV_KEY == ev.type)&&(BTN_TOUCH == ev.code)&&(1 == ev.value))
	{
	x0 = x;
	y0 = y;
	}
	else if((EV_KEY == ev.type)&&(BTN_TOUCH == ev.code)&&(0 == ev.value))
	{
	x0 = x1;
	y0 = y1;
	}
	{
	if ((x1>x)&&(y1>y)&&(y1<y))
	printf("right");
	}
	{
	if ((x1<x)&&(y1>y)&&(y1<y))
	printf("left");
	}
	{
	if ((x1<x)&&(x1>x)&&(y1<y))
	printf("down");
	}
	{
	if ((x1<x)&&(x1>x)&&(y1>y))
	printf("up");
	}
    }


    // 关闭文件
    close(fd_touch);

    return 0;
}
