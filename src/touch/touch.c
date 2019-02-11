#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <linux/input.h>
#include <sys/time.h>
#include <pthread.h>

#include "log.h"
#include "touch_get_event_num.h"
#include "touch_send_event.h"
#include "touch.h"

#define TOUCH_NUM 9

char touch_count = 0;
int fd_event = 0;
char touch_lock = 0;
char touch_type = TOUCH_TYPE_A;

__s32 touch_id_num = 0;
__s32 touch_down_num = 0;

s_touch s_touch_arr[TOUCH_NUM];

void touch_wait_lock(void)
{
    while (touch_lock)
        ;
    touch_lock = 1;
}

void touch_open_lock(void)
{
    touch_lock = 0;
}

int open_event(int event_num)
{

    char name[64];
    int fd_event;
    sprintf(name, "/dev/input/event%d", event_num);
    fd_event = open(name, O_RDWR);

    if (fd_event < 0)
    {
        LOG("open fail%s\n", name);
        return fd_event;
    }
    else
    {
        LOG("open success%s\n", name);
        return fd_event;
    }
}

int close_event(int fd)
{
    return close(fd);
}

void *touch_fun_thread(void *arg)
{
    static count = 0;

    __s32 x, y;

    //printf("thread start\r\n");

    while (1)
    {
        int i = 0;
        for (; i < TOUCH_NUM; i++)
        {
            if (s_touch_arr[i].is_use != 0 && s_touch_arr[i].type != 0)
            {

                if (s_touch_arr[i].step_count < s_touch_arr[i].step)
                {
                    s_touch_arr[i].step_count++;

                    x = s_touch_arr[i].start_x + (s_touch_arr[i].end_x - s_touch_arr[i].start_x) * s_touch_arr[i].step_count / s_touch_arr[i].step;
                    y = s_touch_arr[i].start_y + (s_touch_arr[i].end_y - s_touch_arr[i].start_y) * s_touch_arr[i].step_count / s_touch_arr[i].step;

                    //LOG("%d %d\n", x,y);
                    touch_move(i, x, y);
                }
                else
                {
                    if (s_touch_arr[i].type == TOUCH_MOVE_UP)
                    {
                        touch_up(i);
                    }
                    else if (s_touch_arr[i].type == TOUCH_MOVE_NORMAL)
                    {
                        s_touch_arr[i].type = TOUCH_NORMAL;
                    }
                }
            }
        }

        usleep(20000);
    }
    //touch_wait_lock();

    //touch_open_lock();
}

void start_touch_thread()
{
    int res;
    pthread_t touch_thread;
    int share_int = 10;
    res = pthread_create(&touch_thread, NULL, touch_fun_thread, NULL);
}

void touch_struct_init()
{
    int i = 0;
    for (i = 0; i < TOUCH_NUM; i++)
    {
        s_touch_arr[i].is_use = 0;
        s_touch_arr[i].type = TOUCH_NORMAL;
    }
}

int touch_init(void)
{

    int touch_num = get_touchscreen_event_num(&touch_type);

    if (touch_num != -1)
    {
        LOG("get touch success: event%d ", touch_num);

        //touch_type = TOUCH_TYPE_B;
        if (touch_type == TOUCH_TYPE_A)
        {
            LOG("TOUCH_TYPE_A\r\n");
        }
        else if (touch_type == TOUCH_TYPE_B)
        {
            LOG("TOUCH_TYPE_B\r\n");
        }
    }
    else
    {
        LOG("get touch fail,exit\r\n");
        return -1;
    }

    if ((fd_event = open_event(touch_num)) != -1)
    {
        LOG("open touch success: event%d\r\n", touch_num);
    }
    else
    {
        LOG("open touch fail,exit\r\n");
        return -1;
    }

    touch_struct_init();

    start_touch_thread();

    // s_touch touch;
    // touch.start_x = 100;
    // touch.start_y = 100;
    // touch.end_x = 500;
    // touch.end_y = 100;
    // touch.step = 10;
    // touch.type = TOUCH_MOVE_NORMAL;

    // int touch1 = touch_down(fd_event,&touch);

    // touch.start_x = 100;
    // touch.start_y = 200;
    // touch.end_x = 500;
    // touch.end_y = 200;
    // touch.step = 100;
    // touch.type = TOUCH_MOVE_NORMAL;
    // int touch2 = touch_down(fd_event,&touch);

    // if(close_event(fd_event) != -1)
    // {
    //     LOG("close touch success: event%d\r\n", touch_num);
    // }else{
    //     LOG("close touch fail,exit\r\n");
    //     return -1;
    // }

    return fd_event;
}

int touch_down(s_touch *touch)
{
    int i;
    char is_touch_down = 0,ret = 0;

    if (touch_type == TOUCH_TYPE_A)
    {
        for (i = 0; i < TOUCH_NUM; i++)
        {
            if (s_touch_arr[i].is_use == 0x00)
            {
                if(is_touch_down == 0)
                {
                    is_touch_down = 1;
                    touch_count++;
                    memcpy(&s_touch_arr[i], touch, sizeof(s_touch));
                    s_touch_arr[i].is_use = 0x01;
                    s_touch_arr[i].step_count = 0;
                    ret = i;

                    s_touch_arr[i].now_x = s_touch_arr[i].start_x;
                    s_touch_arr[i].now_y = s_touch_arr[i].start_y;

                    send_event(fd_event, EV_ABS, ABS_MT_PRESSURE, 100);
                    send_event(fd_event, EV_ABS, ABS_MT_POSITION_X, s_touch_arr[i].start_x);
                    send_event(fd_event, EV_ABS, ABS_MT_POSITION_Y, s_touch_arr[i].start_y);
                    send_event(fd_event, EV_ABS, ABS_MT_TRACKING_ID, i);
                    send_event(fd_event, EV_SYN, SYN_MT_REPORT, 0);
                }

            }
            else
            {
                send_event(fd_event, EV_ABS, ABS_MT_PRESSURE, 100);
                send_event(fd_event, EV_ABS, ABS_MT_POSITION_X, s_touch_arr[i].now_x);
                send_event(fd_event, EV_ABS, ABS_MT_POSITION_Y, s_touch_arr[i].now_y);                
                send_event(fd_event, EV_ABS, ABS_MT_TRACKING_ID, i);
                send_event(fd_event, EV_SYN, SYN_MT_REPORT, 0);
            }
        }

        if(touch_count == 1)
        {
            send_event(fd_event, EV_KEY, BTN_TOUCH, 1);
        }

        send_event(fd_event, EV_SYN, SYN_REPORT, 0);
    }
    else if (touch_type == TOUCH_TYPE_B)
    {
        for (i = 0; i < TOUCH_NUM; i++)
        {
            if (s_touch_arr[i].is_use == 0x00)
            {
                memcpy(&s_touch_arr[i], touch, sizeof(s_touch));
                s_touch_arr[i].is_use = 0x01;
                s_touch_arr[i].step_count = 0;

                send_event(fd_event, EV_ABS, ABS_MT_SLOT, i);
                send_event(fd_event, EV_ABS, ABS_MT_TRACKING_ID, touch_id_num + i);
                send_event(fd_event, EV_ABS, ABS_MT_POSITION_X, s_touch_arr[i].start_x);
                send_event(fd_event, EV_ABS, ABS_MT_POSITION_Y, s_touch_arr[i].start_y);
                send_event(fd_event, EV_ABS, ABS_MT_PRESSURE, 100);
                send_event(fd_event, EV_SYN, SYN_REPORT, 0);

                return i;
            }
        }
    }

    return ret;
}

int touch_move(int id, __s32 x, __s32 y)
{
    int i;

    s_touch_arr[id].now_x = x;
    s_touch_arr[id].now_y = y;

    if (touch_type == TOUCH_TYPE_A)
    {
        for (i = 0; i < TOUCH_NUM; i++)
        {

            if (s_touch_arr[i].is_use == 0x01)
            {
                if (id == i)
                {
                    send_event(fd_event, EV_ABS, ABS_MT_PRESSURE, 100);
                    send_event(fd_event, EV_ABS, ABS_MT_POSITION_X, x);
                    send_event(fd_event, EV_ABS, ABS_MT_POSITION_Y, y);
                    send_event(fd_event, EV_ABS, ABS_MT_TRACKING_ID, i);
                    send_event(fd_event, EV_SYN, SYN_MT_REPORT, 0);
                }
                else
                {
                    send_event(fd_event, EV_ABS, ABS_MT_PRESSURE, 100);
                    send_event(fd_event, EV_ABS, ABS_MT_POSITION_X, s_touch_arr[i].now_x);
                    send_event(fd_event, EV_ABS, ABS_MT_POSITION_Y, s_touch_arr[i].now_y);
                    send_event(fd_event, EV_ABS, ABS_MT_TRACKING_ID, i);
                    send_event(fd_event, EV_SYN, SYN_MT_REPORT, 0);
                }
            }
        }
        send_event(fd_event, EV_SYN, SYN_REPORT, 0);
    }
    else if (touch_type == TOUCH_TYPE_B)
    {
        if (s_touch_arr[id].is_use != 0x00)
        {

            send_event(fd_event, EV_ABS, ABS_MT_SLOT, id);
            send_event(fd_event, EV_ABS, ABS_MT_POSITION_X, x);
            send_event(fd_event, EV_ABS, ABS_MT_POSITION_Y, y);
            send_event(fd_event, EV_ABS, ABS_MT_PRESSURE, 100);
            send_event(fd_event, EV_SYN, SYN_REPORT, 0);
        }
    }
}

int touch_up(int id)
{
    int i;

    if (touch_type == TOUCH_TYPE_A)
    {
        for (i = 0; i < TOUCH_NUM; i++)
        {

            if (s_touch_arr[i].is_use == 0x01)
            {
                if (id == i)
                {
                    touch_count--;
                    s_touch_arr[id].is_use = 0x00;
                }else
                {
                    send_event(fd_event, EV_ABS, ABS_MT_PRESSURE, 100);
                    send_event(fd_event, EV_ABS, ABS_MT_POSITION_X, s_touch_arr[i].now_x);
                    send_event(fd_event, EV_ABS, ABS_MT_POSITION_Y, s_touch_arr[i].now_y);
                    send_event(fd_event, EV_ABS, ABS_MT_TRACKING_ID, i);
                    send_event(fd_event, EV_SYN, SYN_MT_REPORT, 0);
                }
            }
        }

        if(touch_count == 0)
        {
            send_event(fd_event, EV_SYN, SYN_MT_REPORT, 0);
            send_event(fd_event, EV_KEY, BTN_TOUCH, 0);
        }
        send_event(fd_event, EV_SYN, SYN_REPORT, 0);
    }
    else if (touch_type == TOUCH_TYPE_B)
    {
        if (s_touch_arr[id].is_use != 0x00)
        {
            s_touch_arr[id].is_use = 0x00;

            send_event(fd_event, EV_ABS, ABS_MT_SLOT, id);
            send_event(fd_event, EV_ABS, ABS_MT_TRACKING_ID, -1);
            send_event(fd_event, EV_SYN, SYN_REPORT, 0);
        }
    }
}
