#include"ev.h"
#include"lcd.h"
#include<time.h>

unsigned int game[4][4] = {0}; //游戏矩阵4*4  ,对应数字代表图片的显示

/*
	刷新游戏矩阵中的图片
*/
void refresh()
{
	int i,j;
	for(i = 0;i < 4;i++)
	{
		for(j = 0;j < 4;j++)
		{
			char buf[100] = {0};
			sprintf(buf,"pic/%d.bmp",game[i][j]);
			puts(buf);
			draw_bmp(160+j*120,i*120,buf);
		}
	}
}

/*
	在随机的一个空位置显示2/4
*/
void rand_site()
{
	int x = time(NULL);
	srand(x);
	unsigned char  hole[32] = {0};
	int num = 0;
	int i,j;
	for(i = 0;i < 4;i++)
	{
		for(j = 0;j < 4;j++)
		{
			if(game[i][j] == 0)
			{
				hole[num*2] = i;
				hole[num*2+1] = j;
				num++;
			}
		}
	}
	
	x = rand()%num;//随机一个空位
	
	i = (rand()%2 + 1)*2;//随机2/4
	
	game[hole[x*2]][hole[x*2+1]] = i;//对应矩阵的位置填入
}

/*滑动处理
	dir: 1左划处理
		  2右划处理
		  3上划处理
		  4下划处理

*/
void Sliding_Handle(int dir)
{
	int i,j,k;
	for(i = 0;i < 3;i++)//合移合
	{
		for(j = 0;j < 4;j++)
		{
			int num = 0;
			for(k = 0;k < 4;k++)
			{
				if(i%2 == 0)//移动
				{
					switch(dir)
					{
						case 1:  //左划
							if(game[j][k] != 0)
							{
								game[j][num] = game[j][k];
								if(num != k)
								{
									game[j][k] = 0;
								}
								num++;
							}
							break;
						case 2:  //右划
							if(game[j][3-k] != 0)
							{
								game[j][3-num] = game[j][3-k];
								if(num != k)
								{
									game[j][3-k] = 0;
								}
								num++;
							}
							break;
						case 3: //上划
							if(game[k][j] != 0)
							{
								game[num][j] = game[k][j];
								if(num != k)
								{
									game[k][j] = 0;
								}
								num++;
							}
							break;
						case 4: //下划
							if(game[3-k][j] != 0)
							{
								game[3-num][j] = game[3-k][j];
								if(num != k)
								{
									game[3-k][j] = 0;
								}
								num++;
							}
							break;
					}
				}
				else//合并
				{
					if(k == 3)
					{
						break;
					}
					switch(dir)
					{
						case 1:  //上划
							if(game[j][k] == game[j][k+1])
							{
								game[j][k] *= 2;
								game[j][k+1] = 0;
							}
							break;
						case 2:  //下划
							if(game[j][3-k] == game[j][3-k-1])
							{
								game[j][3-k] *= 2;
								game[j][3-k-1] = 0;
							}
							break;
						case 3: //左划
							if(game[k][j] == game[k+1][j])
							{
								game[k][j] *= 2;
								game[k+1][j] = 0;
							}
							break;
						case 4: //右划
							if(game[3-k][j] == game[3-k-1][j])
							{
								game[3-k][j] *= 2;
								game[3-k-1][j] = 0;
							}
							break;
					}
				}
			}
		}
	}
}