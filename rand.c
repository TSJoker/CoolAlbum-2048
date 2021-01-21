//生成随机数
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
	int a,b;
    srand(time(NULL));
    for (int i = 0; i < 6; i++)
	{
	a = rand()%33+1;
	printf("%d\t",a);
	}	
	for(int j = 0;j < 1;j++)
	{    	
	b = rand()%16+1;
        printf("%d\t",b); 
  	}
    return 0;
	
}
