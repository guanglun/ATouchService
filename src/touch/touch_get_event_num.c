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
int fd = 0;
struct input_event event;
char get_touch_type = TOUCH_TYPE_A;
char is_loop_read = 1;

void *touch_read_slot_thread(void *arg)
{

    int res = -1;
    is_loop_read = 1;
#ifdef PRINTF_LOG
    LOG("READ THREAD START\r\n");
#endif
    while (is_loop_read)
    {
        res = read(fd, &event, sizeof(event));
        if (res < (int)sizeof(event))
        {
#ifdef PRINTF_LOG
            LOG(stderr, "could not get event\n");
#endif
            is_loop_read = 0;
        }
        else
        {
#ifdef PRINTF_LOG
            LOG("%04x %04x %08x\r\n", event.type, event.code, event.value);
#endif
            if (event.type == EV_ABS && event.code == ABS_MT_SLOT)
            {
                get_touch_type = TOUCH_TYPE_B;
                is_loop_read = 0;
            }
        }
    }
//close(fd);
#ifdef PRINTF_LOG
    LOG("READ THREAD EXIT\r\n");
#endif
    return 0;
}

int get_touchscreen_event_num(char *touch_type)
{
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

            if (ret != -1)
            {
                //*touch_type = TOUCH_TYPE_B;
                //return ret;

                i = 32;
                pthread_create(&touch_thread, NULL, touch_read_slot_thread, NULL);

                usleep(5000);
#ifdef PRINTF_LOG
                LOG("Write SLOT START\r\n");
#endif
                send_event(fd, EV_ABS, ABS_MT_SLOT, 0);
                send_event(fd, EV_ABS, ABS_MT_TRACKING_ID, 0);
                send_event(fd, EV_KEY, BTN_TOUCH, 1);
                send_event(fd, EV_ABS, ABS_MT_POSITION_X, 0);
                send_event(fd, EV_ABS, ABS_MT_POSITION_Y, 0);
                send_event(fd, EV_ABS, ABS_MT_PRESSURE, 100);
                send_event(fd, EV_SYN, SYN_REPORT, 0);

                send_event(fd, EV_ABS, ABS_MT_SLOT, 1);
                send_event(fd, EV_ABS, ABS_MT_TRACKING_ID, 1);
                send_event(fd, EV_ABS, ABS_MT_POSITION_X, 0);
                send_event(fd, EV_ABS, ABS_MT_POSITION_Y, 0);
                send_event(fd, EV_ABS, ABS_MT_PRESSURE, 100);
                send_event(fd, EV_SYN, SYN_REPORT, 0);

                send_event(fd, EV_ABS, ABS_MT_SLOT, 0);
                send_event(fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
                send_event(fd, EV_SYN, SYN_REPORT, 0);

                send_event(fd, EV_ABS, ABS_MT_SLOT, 1);
                send_event(fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
                send_event(fd, EV_KEY, BTN_TOUCH, 0);
                send_event(fd, EV_SYN, SYN_REPORT, 0);


#ifdef PRINTF_LOG
                LOG("Write SLOT END\r\n");
#endif
            }

            usleep(5000);
            *touch_type = get_touch_type;
            is_loop_read = 0;

            close(fd);
        }
    }
    return ret;
}