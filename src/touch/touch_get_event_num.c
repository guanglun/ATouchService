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
#include "log.h"

#define PRINTF_LOG

#define BITS_PER_LONG 32

#define BIT_WORD(nr) ((nr) / BITS_PER_LONG)

static int test_bit2(int nr, const volatile unsigned long *addr)
{
    return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG - 1)));
}

#define test_bit(bit) (mask[(bit) / 8] & (1 << ((bit) % 8)))

void get_event_size(int fd, int *size_x, int *size_y);

char get_type(int event_num)
{
    char input_str[1024*8];
    char recv_str[1024*8];
    char get_touch_type = TOUCH_TYPE_ERROR;
    FILE *fp;  

    sprintf(input_str,"getevent -lp /dev/input/event%d",event_num); 
    fp = popen(input_str, "r"); 
    while(fgets(recv_str,sizeof(recv_str),fp) != NULL)
    {
        //LOG("%s",recv_str); 
        if(strstr(recv_str,"ABS_MT_SLOT") != NULL)
        {
            get_touch_type = TOUCH_TYPE_B;
        }

        if(strstr(recv_str,"ABS_MT_POSITION_X") != NULL)
        {
            if(get_touch_type == TOUCH_TYPE_ERROR)
            {
                get_touch_type = TOUCH_TYPE_A;
            }
        }        
    }
        
    pclose(fp);
    return get_touch_type;
}

int get_touchscreen_event_num(char *touch_type,int *size_x, int *size_y)
{
    int fd = 0;
    int ret = -1,get_type_ret = TOUCH_TYPE_ERROR;
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
                        get_type_ret = get_type(i);
                        if(get_type_ret != TOUCH_TYPE_ERROR)
                        {
                            get_event_size(fd, size_x, size_y);
                            *touch_type = get_type_ret;
                            ret = i;
                        }
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
    return ret;
}

void get_event_size(int fd, int *size_x, int *size_y)
{
    unsigned char abs_b[ABS_MAX / 8 + 1];
    struct input_absinfo abs_feat;
    int yalv;
    
    ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(abs_b)), abs_b);

    //printf("Supported Absolute axes:\r\n");

    for (yalv = 0; yalv < ABS_MAX; yalv++)
    {
        if (test_bit2(yalv, abs_b))
        {

            if (ioctl(fd, EVIOCGABS(yalv), &abs_feat))
            {
                perror("evdev EVIOCGABS ioctl");
            }

            //printf("  Absolute axis 0x%02x ", yalv);
            switch (yalv)
            {
            case ABS_X:
                //printf("(X Axis) ");
                *size_x = abs_feat.maximum;
                break;
            case ABS_Y:
                //printf("(Y Axis) ");
                *size_y = abs_feat.maximum;
                break;
            default:
                //printf("(Unknown abs feature)");
                break;
            }

            // printf("%d (min:%d max:%d flat:%d fuzz:%d)",
            //        abs_feat.value,
            //        abs_feat.minimum,
            //        abs_feat.maximum,
            //        abs_feat.flat,
            //        abs_feat.fuzz);
            // printf("\r\n");
        }
    }
}