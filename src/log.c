#include <stdio.h>

#include "log.h"

void log_byte(char *buf,int len)
{
    int i = 0;
    for(;i<len;i++)
    {
        LOG("%02X ",*(buf+i));
    }
    LOG("\r\n");
}

unsigned short char2short(char c1,char c2)
{
    unsigned short tmp = 0;
    tmp = (unsigned short)(c1 << 8);
    tmp |= (unsigned short)(c2);
    return tmp;
}