//循环读取触摸屏的数据
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>

#include <fcntl.h>

#include <unistd.h>

#include <stdio.h>

#include <sys/mman.h>

#include <stdlib.h>

#include <linux/input.h>
#define MOVE_UP 1

#define MOVE_DOWN 2

#define MOVE_LEFT 3

#define MOVE_RIGHT 4
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
struct input_event ev;
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
}
        



