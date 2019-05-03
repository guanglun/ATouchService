#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <linux/input.h>
#include <pthread.h>

#include "touch_get_event_num.h"
#include "touch_send_event.h"

//#define PRINTF_LOG

char get_touch_type = TOUCH_TYPE_A;

char get_type(void)
{
    char recv_str[1024*8];
    FILE *fp;  

    //printf("getevent -lp\r\n"); 
    fp = popen("getevent -lp", "r"); 
    while(fgets(recv_str,sizeof(recv_str),fp) != NULL)
    {
        //printf("%s",recv_str); 
        if(strstr(recv_str,"ABS_MT_SLOT") != NULL)
        {
            pclose(fp);
            return TOUCH_TYPE_B;
        }
    }
        
    pclose(fp);
    return TOUCH_TYPE_A;
}

int get_touchscreen_event_num(char *touch_type)
{
    int fd = 0;
    int ret = -1;
    char name[64]; /* RATS: Use ok, but could be better */
    char buf[256] = {
        0,
    };                                  /* RATS: Use ok */
    unsigned char mask[EV_MAX / 8 + 1]; /* RATS: Use ok */
    int version;
    pthread_t touch_thread;
    int rc;
    int i, j;
    char *tmp;

    *touch_type = TOUCH_TYPE_A;

#define test_bit(bit) (mask[(bit) / 8] & (1 << ((bit) % 8)))

    for (i = 0; i < 32; i++)
    {
        sprintf(name, "/dev/input/event%d", i);
        if ((fd = open(name, O_RDWR, 0)) >= 0)
        {

            ioctl(fd, EVIOCGVERSION, &version);
            ioctl(fd, EVIOCGNAME(sizeof(buf)), buf);
            ioctl(fd, EVIOCGBIT(0, sizeof(mask)), mask);

#ifdef PRINTF_LOG
            LOG("%s\n", name);
            LOG("    evdev version: %d.%d.%d\n",
                   version >> 16, (version >> 8) & 0xff, version & 0xff);
            LOG("    name: %s\n", buf);
            LOG("    features:");
#endif

            for (j = 0; j < EV_MAX; j++)
            {
                if (test_bit(j))
                {
                    const char *type = "unknown";
                    switch (j)
                    {
                    case EV_KEY:
                        type = "keys/buttons";
                        break;
                    case EV_REL:
                        type = "relative";
                        break;
                    case EV_ABS:
                        type = "absolute";
                        ret = i;
                        break;
                    case EV_MSC:
                        type = "reserved";
                        break;
                    case EV_LED:
                        type = "leds";
                        break;
                    case EV_SND:
                        type = "sound";
                        break;
                    case EV_REP:
                        type = "repeat";
                        break;
                    case EV_FF:
                        type = "feedback";
                        break;
                    }
#ifdef PRINTF_LOG
                    LOG(" %s", type);
#endif
                }
            }
#ifdef PRINTF_LOG
            LOG("\n");
#endif

            close(fd);
        }
    }


    *touch_type = get_type();

    return ret;
}