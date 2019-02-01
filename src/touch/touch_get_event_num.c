#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <linux/input.h>

//#define PRINTF_LOG

struct input_event event;

int get_touchscreen_event_num(void)
{
    int           ret = -1;
    char          name[64];           /* RATS: Use ok, but could be better */
    char          buf[256] = { 0, };  /* RATS: Use ok */
    unsigned char mask[EV_MAX/8 + 1]; /* RATS: Use ok */
    int           version;
    int           fd = 0;
    int           rc;
    int           i, j;
    char          *tmp;
 
#define test_bit(bit) (mask[(bit)/8] & (1 << ((bit)%8)))
 
    for (i = 0; i < 32; i++) {
        sprintf(name, "/dev/input/event%d", i);
        if ((fd = open(name, O_RDONLY, 0)) >= 0) {

            ioctl(fd, EVIOCGVERSION, &version);
            ioctl(fd, EVIOCGNAME(sizeof(buf)), buf);
            ioctl(fd, EVIOCGBIT(0, sizeof(mask)), mask);

#ifdef PRINTF_LOG            
            printf("%s\n", name);
            printf("    evdev version: %d.%d.%d\n",
                   version >> 16, (version >> 8) & 0xff, version & 0xff);
            printf("    name: %s\n", buf);
            printf("    features:");
#endif         

            for (j = 0; j < EV_MAX; j++) {
                if (test_bit(j)) {
                    const char *type = "unknown";
                    switch(j) {
                    case EV_KEY: type = "keys/buttons"; break;
                    case EV_REL: type = "relative";     break;
                    case EV_ABS: type = "absolute";   ret = i;  break;
                    case EV_MSC: type = "reserved";     break;
                    case EV_LED: type = "leds";         break;
                    case EV_SND: type = "sound";        break;
                    case EV_REP: type = "repeat";       break;
                    case EV_FF:  type = "feedback";     break;
                    }
#ifdef PRINTF_LOG                      
                    printf(" %s", type);
#endif                    
                }
            }
#ifdef PRINTF_LOG               
            printf("\n");
#endif              
            close(fd);
        }
    }
    return ret;
}