#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <linux/input.h>

#include "touch_send_event.h"
#include "log.h"

char rotation = 0;
int width = 0,heigth = 0;

void set_rotation(char ro,int w,int h)
{
    rotation = ro;
    width = w;
    heigth = h;
}

int send_event(int fd,__u16 type, __u16 code, __s32 value)
{
    struct input_event event;
    int ret = 0;

    event.type = type;
    event.code = code;
    event.value = value;

    if(code == ABS_MT_POSITION_X)
    {
        if(rotation == 1)
        {
            event.code = ABS_MT_POSITION_Y;
        }else if(rotation == 3)
        {
            event.code = ABS_MT_POSITION_Y;
            event.value = heigth - value;
        }
    }else if(code == ABS_MT_POSITION_Y)
    {
        if(rotation == 1)
        {
            event.code = ABS_MT_POSITION_X;
            event.value = width - value;
        }else if(rotation == 3)
        {
            event.code = ABS_MT_POSITION_X;
        }
    }

    LOG("%04x %04x %08x\r\n", event.type, event.code, event.value);

    ret = write(fd, &event, sizeof(event));
    if (ret < (ssize_t)sizeof(event))
    {
        LOG("write event failed, %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

