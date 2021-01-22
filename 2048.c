#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>

#include <fcntl.h>

#include <unistd.h>

#include <stdio.h>

#include <sys/mman.h>

#include <stdlib.h>

#include <linux/input.h>

#define LCD_WIDTH 800    //屏幕宽度

#define LCD_HEIGHT 480   //屏幕高度

#define ITEM_NUM  4   // 创建4行4列的矩阵

#define ITEM_WIDTH  100     //  图片宽100

#define ITEM_HEIGHT 100   //    图片高100

#define BLANK_SIZE   5

#define MATRIX_X0   (LCD_WIDTH - (ITEM_WIDTH + BLANK_SIZE)*ITEM_NUM)/2

#define MATRIX_Y0   (LCD_HEIGHT - (ITEM_HEIGHT + BLANK_SIZE)*ITEM_NUM)/2

void bmp_display(int x0, int y0,const char *bmp_file);

int *plcd = NULL;

int matrix_2048_bak[4][4];

int matrix_2048[4][4] = 
{
                   0, 0, 0, 0,

                   0, 0, 0, 0,

                   0, 0, 0, 0,

                   0, 0, 0, 0

};

void print_matrix_2048()

{
int i,j;

         for (i = 0; i < 4; i++)

         {
for (j = 0; j < 4 ; j++)

                   {
                            printf("%d  " ,matrix_2048[i][j]);

                   }

                   printf("\n");

         }

}

int is_change()

{
         int i, j ;

         for (i = 0; i < 4; i++)

         {
                   for (j = 0; j < 4; j++)

                   {
                            if (matrix_2048[i][j] != matrix_2048_bak[i][j])

                            {
                                     return 1;

                            }

                   }

         }

         return 0;

}

void LCD_Draw_Point(int x, int y, int color)

{
         if (x >= 0 &&  x < 800 && y >= 0 && y < 480)

         {
                    *(plcd + 800*y + x) = color;

         }

}

//LCD_Draw_JuXing: 在屏幕点(x0,y0)处画一个长为w,高为h的矩形

void LCD_Draw_JuXing(int x0, int y0, int w, int h ,int color)

{
         //假设矩形内任意点的坐标为(x,y)

         int x, y;

         for (y = y0; y < y0 + h; y++)

         {
                   for (x = x0; x < x0 + w; x++)

                   {
                            LCD_Draw_Point(x, y, color);

                   }

         }

}

void LCD_Draw_Matrix()

{
         int i, j;

         int x0, y0; //每个棋子矩形左上顶点的坐标

         for (i = 0; i < ITEM_NUM; i++)

         {
                   for (j = 0; j < ITEM_NUM ; j++)

                   {
                            y0 = MATRIX_Y0 + (ITEM_HEIGHT + BLANK_SIZE)*i;

                            x0 = MATRIX_X0 + (ITEM_WIDTH + BLANK_SIZE)*j;

                            if (matrix_2048[i][j] == 0)

                            {
                                     LCD_Draw_JuXing(x0, y0, ITEM_WIDTH, ITEM_HEIGHT, 0x00ff00);

                            }

                            else

                            {
                                     char filename[32];

                                     sprintf(filename, "m%d.bmp", matrix_2048[i][j]);

                                     bmp_display(x0,  y0, filename);

                            }

                   }       

         }

}

void bmp_display(int x0, int y0,const char *bmp_file)

{
         int fd;

         unsigned char buf[4];

         int ret;

         fd = open(bmp_file, O_RDONLY);

         if (fd == -1)

         {
                   printf("failed to open %s\n", bmp_file);

                   return ;

         }

         lseek(fd, 0 ,SEEK_SET);

         read(fd, buf, 2);

if (buf[0] == 0x42 && buf[1] == 0x4d )

         {
                   printf("BMP file\n");

         }

         else

         {
                   printf("Not BMP file, GOOD BYE\n");

                   close(fd);

                   return ;

         }

         int width,height;

         int depth;

         lseek(fd, 0x12, SEEK_SET);

         read(fd, buf, 4);

         width =  (buf[0]) | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);

         lseek(fd, 0x16, SEEK_SET);

         read(fd, buf, 4);

         height =  (buf[0]) | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);

         printf("%d X %d\n", width, height);

         lseek(fd, 0x1c, SEEK_SET);

         read(fd, buf, 2);

         depth = (buf[0]) | (buf[1] << 8);

         if (depth != 24 && depth != 32)

         {
                   printf("Not Support\n");

                   close(fd);

                   return ;

         }       

         int bytes_per_line = abs(width) * (depth / 8);

         int laizi = 0;

         if (bytes_per_line % 4 != 0)

         {
                   laizi = 4 - bytes_per_line % 4;

         }

         int bytes_line = bytes_per_line + laizi;

         int size = bytes_line * abs(height);

         unsigned char *p =           malloc(size);

         lseek(fd, 54, SEEK_SET);

         read(fd, p, size);

         unsigned char b ,g, r ,a;

         int color;

         int i = 0;

         int x;

         int y;

         for (y = 0; y < abs(height); y++)

         {
                   for (x = 0 ;x  < abs(width); x++ )

                   {       

                            b = p[i++];

                            g = p[i++];

                            r = p[i++];

                            if (depth == 32)

                            {
                                     a = p[i++];

                            }

                            else

                            {
                                     a = 0;

                            }

                            color = (a << 24) | (r << 16) | (g << 8) | (b);   

                            LCD_Draw_Point( width > 0 ? x0 + x : x0 + abs(width) - 1 -x,

                                                                 height > 0 ? y0 + height - 1 - y : y0 + y,

                                                                 color);

                   }

                   i = i + laizi;

         }       

         free(p);

         close(fd);

}

int get_zero_num()

{
         int n = 0;

         int i, j;

         for (i = 0; i < ITEM_NUM ;i++)

         {
                   for (j = 0; j < ITEM_NUM; j++)

                   {
                            if (matrix_2048[i][j] == 0)

                            {
                                     n++;

                            }

                   }

         }

         return n;

}

void fill_random(void)

{
         int zero_num = get_zero_num(); //获取数组中值为0的个数

         int pos; //随机的位置的编号，应该要在[0, zero_num)

         pos = random() % zero_num ; // => pos =>[0,zero_num)

         int i,j;

         int n = 0; //第几个0

         for (i = 0; i < ITEM_NUM; i++)

         {
                   for (j = 0; j < ITEM_NUM; j++)

                   {
                            if (matrix_2048[i][j] == 0)

                            {
                                     if (n == pos)

                                     {
                                               matrix_2048[i][j] = 2;//a[index];

                                               return ;

                                     }

                                     n++;

                            }

                   }

         }

}

#define MOVE_UP 1

#define MOVE_DOWN 2

#define MOVE_LEFT 3

#define MOVE_RIGHT 4

int get_finger_movemenet(void)

{
         int fd;

         int ret;

         fd = open("/dev/input/event0", O_RDONLY);

         //fd = open("/dev/event0", O_RDONLY);

         if (fd == -1)

         {
                   printf("open /dev/event0 failed\n");

                   return -1;

         }

         int x1 = -1, y1 = -1; //在一次滑动过程中，第一个点的坐标

         int x2, y2; //在一次滑动过程中 ，最后一个点的坐标

         struct input_event ev;

         while (1)

         {
                   ret = read(fd, &ev, sizeof(ev));

                   if (ret != sizeof(ev))

                   {
                            continue;

                   }

                   if (ev.type == EV_ABS && ev.code == ABS_X)

                   {
                            if (x1 == -1)

                            {
                                     x1 = ev.value;

                            }

                            x2 = ev.value;

                   }

                   if (ev.type == EV_ABS && ev.code == ABS_Y)

                   {
                            if (y1 == -1)

                            {
                                     y1 = ev.value;

                            }

                            y2 = ev.value;

                   }

                   if (ev.type == EV_ABS && ev.code == ABS_PRESSURE && ev.value == 0)

                   {
                            int delt_x = abs(x2 - x1); // 左右

                            int delt_y = abs(y2 - y1); //上下

                            if (delt_x > 2*delt_y)

                            {
                                     if (x2 > x1)

                                     {
                                               close(fd);

                                               return MOVE_RIGHT;

                                     }

                                     else

                                     {
                                               close(fd);

                                               return MOVE_LEFT;

                                     }

                            }

                            else if (delt_y > 2*delt_x)

                            {
                                     //上下

                                     if (y2 > y1)

                                     {
                                               close(fd);

                                               return MOVE_DOWN;

                                     }

                                     else

                                     {
                                               close(fd);

                                               return MOVE_UP;

                                     }

                            }

                            else

                            {
                                     x1 = -1;

                                     y1 = -1;

                            }       

                   }       

         }

         close(fd);

}

void move_left(void)
{
         int i,j;
         int x,y;
         for (i = 0; i < ITEM_NUM; i++)
         {
                   for (x = 0; x < ITEM_NUM; )
                   {
                            if (matrix_2048[i][x] != 0)
                            {
                                     for (y = x + 1; y < ITEM_NUM; y++)
                                     {
                                               if (matrix_2048[i][y] != 0)
                                               {
                                                        if (matrix_2048[i][x] == matrix_2048[i][y])

                                                        {
                                                                 matrix_2048[i][x] += matrix_2048[i][y];

                                                                 matrix_2048[i][y] = 0;

                                                                 x = y + 1;

                                                                 break;

                                                        }

                                                        else

                                                        {
                                                                 x = y;

                                                        }

                                               }

                                     }

                                     if (y >= ITEM_NUM)

                                     {
                                               break;

                                     }

                            }

                            else

                            {
                                     x++;

                            }

                   }

                   x = 0;

                   for (y = 0 ; y < ITEM_NUM; y++)

                   {
                            if (matrix_2048[i][y] != 0)

                            {
                                     if (x != y)

                                     {
                                               matrix_2048[i][x] = matrix_2048[i][y];

                                               matrix_2048[i][y] = 0;

                                     }

                                     x++;

                            }

                   }

         }

}

void move_right(void)

{
         int i, j;

         int x, y;

         for (i = 0; i < ITEM_NUM; i++)

         {
                   for (x = ITEM_NUM; x >0; )

                   {
                            if (matrix_2048[i][x] != 0)

                            {
                                     for (y = x-1; y>=0; y--)

                                     {
                                               if (matrix_2048[i][y] != 0)

                                               {
                                                        if (matrix_2048[i][x] == matrix_2048[i][y])

                                                        {
                                                                 matrix_2048[i][x] += matrix_2048[i][y];

                                                                 matrix_2048[i][y] = 0;

                                                                 x = y - 1;

                                                                 break;

                                                        }

                                                        else

                                                        {
                                                                 x = y;

                                                        }

                                               }

                                     }

                                     if (y<0)

                                     {
                                               break;

                                     }

                            }

                            else

                            {
                                     x--;

                            }

                   }

                   x = ITEM_NUM-1;

                   for (y = ITEM_NUM-1 ; y >= 0; y--)

                   {
                            if (matrix_2048[i][y] != 0)

                            {
                                                        if (x != y)

                                     {
                                               matrix_2048[i][x] = matrix_2048[i][y];

                                               matrix_2048[i][y] = 0;

                                     }

                                     x--;

                            }

                   }

         }

}

void move_down(void)

{
         int i, j;

         int x, y;

         for (i = 0; i < ITEM_NUM; i++)

         {
                   for (x = 0; x < ITEM_NUM; )

                   {
                            if (matrix_2048[x][i] != 0)

                            {
                                     for (y = x + 1; y < ITEM_NUM; y++)

                                     {
                                               if (matrix_2048[y][i] != 0)

                                               {
                                                        if (matrix_2048[x][i] == matrix_2048[y][i])

                                                        {
                                                                 matrix_2048[x][i] += matrix_2048[y][i];

                                                                 matrix_2048[y][i] = 0;

                                                                 x = y + 1;

                                                                 break;

                                                        }

                                                        else

                                                        {
                                                                 x = y;

                                                        }

                                               }

                                     }

                                     if (y >= ITEM_NUM)

                                     {
                                               break;

                                     }

                            }

                            else

                            {
                                     x++;

                            }

                   }       

  x = 0;

                   for (y = 0; y < ITEM_NUM; y++)

                   {
                            if (matrix_2048[y][i] != 0)

                            {       

                                     if (x != y)

                                     {
                                               matrix_2048[x][i] = matrix_2048[y][i];

                                               matrix_2048[y][i] = 0;

                                     }

                                     x++;

                            }

                   }

         }

}

void move_up(void)

{
         int i, j;

         int x, y;

         for (i = 0; i < ITEM_NUM; i++)

         {
                   for (x = ITEM_NUM; x >=0; )

                   {
                            if (matrix_2048[x][i] != 0)

                            {
for (y = x - 1; y >= 0; y--)

                                     {
                                               if (matrix_2048[y][i] != 0)

                                               {
                                                        if (matrix_2048[x][i] == matrix_2048[y][i])

                                                        {
                                                                 matrix_2048[x][i] += matrix_2048[y][i];

                                                                 matrix_2048[y][i] = 0;

         x = y -1;

                                                                 break;

                                                        }

                                                        else

                                                        {
                                                                 x = y;

                                                        }

                                               }

                                     }

if (y<0)

                                     {
                                               break;

                                     }

                            }

                            else

                            {
                                     x--;

                            }

                   }

                   x = ITEM_NUM-1;

                   for (y = ITEM_NUM-1 ; y >= 0; y--)

                   {
                            if (matrix_2048[y][i] != 0)

                            {
                                     if (x != y)

                                     {
                                               matrix_2048[x][i] = matrix_2048[y][i];

                                               matrix_2048[y][i] = 0;

                                     }

       x--;

                            }

                   }

         }

}

int is_gameover(void)

{
         int x,y;

         for(y=0;y<ITEM_NUM;y++)

                   {       

                            for(x=0;x<ITEM_NUM;x++)

                                     {
                                               if(matrix_2048[x][y]==0)

                                                        {
                                                                 return 0; 

                                                        }

                                               if(x<ITEM_NUM-1 && matrix_2048[x][y]==matrix_2048[x+1][y])

                                                        {
                                                                 return 0;

}

                                               if(y<ITEM_NUM-1 && matrix_2048[x][y]==matrix_2048[x][y+1])

                                                        {
                                                                 return 0;

                                                        }

                                     }

                   }

         return 1;

}

//根据手指滑动方向变换矩阵

void change_matrix(int mv)

{
         if (mv == MOVE_LEFT)

         {
                   move_left();

         }

         else if (mv == MOVE_RIGHT)

         {
              move_right();

         }

         else if (mv == MOVE_UP)

         {
                   move_down();

         }

         else if (mv == MOVE_DOWN)

         {
                   move_up();

         }

}

int main()

{
         //1. 打开文件

         int fd = open("/dev/fb0", O_RDWR);

         if (fd == -1)

         {
                   printf("failed to open /dev/fb0\n");

                   return -1;

         }

printf("open /dev/fb0 successfully\n");

         //2. 操作文件

         plcd = mmap(NULL,

                                     800*480*4,

                                     PROT_WRITE,

                                     MAP_SHARED,

                                     fd,

                                     0);

         if (plcd == MAP_FAILED)

         {
                   printf("failed to mmap\n");

                   return -1;

         }

         srandom(time(NULL)) ;//设置随机数种子

         LCD_Draw_JuXing(0, 0, LCD_WIDTH, LCD_HEIGHT, 0X555555);

         fill_random();

         //is_gameover();

         LCD_Draw_Matrix();

         while (1)

         {
                   int mv = get_finger_movemenet();

                   printf("mv = %d\n", mv);

                   change_matrix(mv);

                  fill_random();

                   LCD_Draw_Matrix();

                   print_matrix_2048();

                   if (is_gameover())

                   {
                            break;

                   }

         }

         //3.关闭文件

         munmap(plcd, 800*480*4);

         close(fd);
}

