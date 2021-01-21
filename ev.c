#include"ev.h"

/*
	获取手指点击位置
	x:一个指针，用于保存获取到的x轴坐标
	y:一个指针，用于保存获取到的y轴坐标

*/
void get_touch(int *x,int *y)
{
	int fd = open("/dev/input/event0",O_RDONLY);
	if(fd == -1)
	{
		perror("open ev err");
		return ;
	}
	
	struct input_event ev = {0};
	
	while(1)
	{
		int r = read(fd,&ev,sizeof(ev));
		if(r != sizeof(ev))
		{
			continue;
		}
		
		if(ev.type == EV_ABS)
		{
			if(ev.code == ABS_X)
			{
				*x = ev.value;
			}
			if(ev.code == ABS_Y)
			{
				*y = ev.value;
			}
			if(ev.code == ABS_PRESSURE && ev.value == 0)
			{
				break;
			}
		}
		
		if(ev.type == EV_KEY  && ev.code == BTN_TOUCH)
		{
			if(ev.value == 0)
			{
				break;
			}
		}			
		
	}

	return ;
}


/*
	获取手指滑动方向
	返回值：
		1：左
		2：右
		3：上
		4：下
*/
int get_direction()
{
	int fd = open("/dev/input/event0",O_RDONLY);
	if(fd == -1)
	{
		perror("open ev err");
		return -1;
	}
	
	struct input_event ev = {0};
	int x0 =  -1,y0 = -1,x = -1,y = -1;
	
	
	while(1)
	{
		int r = read(fd,&ev,sizeof(ev));
		if(r != sizeof(ev))
		{
			continue;
		}
		
		if(ev.type == EV_ABS)
		{
			if(ev.code == ABS_X)
			{
				if(x0 == -1)
				{
					x0 = ev.value;
				}
				x = ev.value;
			}
			if(ev.code == ABS_Y)
			{
				if(y0 == -1)
				{
					y0 = ev.value;
				}
				y = ev.value;
			}
			if(ev.code == ABS_PRESSURE && ev.value == 0)
			{
				if(x != -1 && y != -1)
				break;
			}
		}
		
		if(ev.type == EV_KEY  && ev.code == BTN_TOUCH)
		{
			if(ev.value == 0)
			{
				break;
			}
		}			
		
	}
	
	printf("x0 = %d  x = %d  y0 = %d  y = %d\n",x0,x,y0,y);
	
	int x_v = (x0-x > 0)?(x0-x):(-(x0-x));
	int y_v = (y0-y > 0)?(y0-y):(-(y0-y));
	

	
	if(x_v > y_v)
	{
		if(x0-x > 0)
		{
			return 1;
		}
		return 2;
	}
	else
	{
		if(y0-y > 0)
		{
			return 3;
		}
		return 4;
	}

}



