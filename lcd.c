#include"lcd.h"

int *plcd = NULL;

/*
	初始化lcd
	pathname:屏幕设备文件名（带路径）
*/
int init_lcd(char *pathname)
{
	int fd = open("/dev/fb0",O_RDWR);
	if(fd == -1)
	{
		perror("open fb error");
		exit(-1);
	}
	
	plcd = mmap(NULL,800*480*4,PROT_READ | PROT_WRITE, MAP_SHARED,fd,0);
	if(plcd == MAP_FAILED)
	{
		perror("mmap error");
		exit(-1);
	}
	
	return fd;
}


void uninit(int fd)
{
	munmap(plcd,800*480*4);
	close(fd);
}

/*
	把屏幕上对应(x.y)坐标的点的颜色显示为color
	x:x轴坐标  （坐标轴心为 显示屏左上角）
	y:y轴坐标   
	color: 量化后的颜色值
*/
void draw_point(int x,int y,int color)
{
	if(x >=0  && x < 800  && y >= 0 && y < 480)
	{
		*(plcd + 800*y + x) = color;
	}
}

void clear_lcd()
{
	int i,j;
	for(i = 0;i < 480;i++)
	{
		for(j = 0;j < 800;j++)
		{
				draw_point(j,i,0x42ceae);
		}
	}
}




/*
	在屏幕的（x,y）坐标处显示一张bmp图片
	x:x轴坐标
	y:y轴坐标  
	bmpname:图片的名字（带路径）
*/
void draw_bmp(int x,int y,char *bmpname)
{
	int fd = open(bmpname,O_RDONLY);
	if(fd == -1)
	{
		perror("open pic error");
		return ;
	}
	
	int high,wid;
	lseek(fd,0x12,SEEK_SET);
	read(fd,&wid,4);
	read(fd,&high,4);
	
	short dep;
	lseek(fd,0x1c,SEEK_SET);
	read(fd,&dep,2);
	
	//printf("high = %d wid = %d  dep = %hd\n",high,wid,dep);
	
	lseek(fd,0x36,SEEK_SET);
	
	if(dep == 24)
	{
		int setdata = 4 - wid*dep/8%4;
		if(setdata == 4)
		{
			setdata = 0;
		}
		int line_size = (wid*dep/8 + setdata);
	
		unsigned char buf[high*line_size];
		int ret = read(fd,buf,line_size*high);
		if(ret != line_size*high)
		{
			printf("read err %d\n",ret);
		}

		int i,j,num = 0,color;
		for(i = 0;i < high;i++)
		{
			for(j = 0;j < wid;j++)
			{
				color = buf[num++] | (buf[num++] << 8) | (buf[num++] << 16);
				draw_point(j+x,high-i-1+y,color);
			}
			num+=setdata;
		}
	}
	else if(dep == 32)//32位bmp图片
	{
		int buf[high*wid];
		read(fd,buf,high*wid*4);
		int i,j;
		for(i = 0;i < high;i++)
		{
			for(j = 0;j < wid;j++)
			{
				draw_point(j+x,high-i-1+y,buf[i*wid + j]);
			}
		}	
	}
	close(fd);
}









