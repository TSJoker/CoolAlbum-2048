#include"ev.h"
#include"lcd.h"
#include"game.h"


void prin_g()
{
	int i,j;
	for(i = 0;i < 4;i++)
	{
		for(j = 0;j < 4;j++)
		{
			printf("%d\t",game[i][j]);
		}
		puts("");
	}
	puts("**************************************************");
}


int main()
{
	int fd = init_lcd("/dev/fb0");//初始化lcd
	clear_lcd();//刷屏
	rand_site();
	rand_site();//随机两个位置出现2/4
	refresh();//刷新游戏界面

	while(1)
	{	
		int x = get_direction();
		printf("%d\n",x);
		Sliding_Handle(x);
		rand_site();
		refresh();
	}
	
	
}