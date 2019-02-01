#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <linux/input.h>

int fd_event = 0;

void open_event(int event_num)
{
    
    char name[64]; 

    sprintf(name, "/dev/input/event%d", event_num);
    fd_event = open(name, O_RDWR);

    if(fd_event < 0) {
        printf( "open fail%s\n",name);
        return fd_event;
    }else{
        printf( "open success%s\n",name);
        return fd_event;
    }
}

void close_event()
{
    close(fd_event);
}