#include <stdio.h>

#include "touch_get_event_num.h"

int main()
{
    printf("\r\n==============start=============\r\n");
    printf("ATouchService is runing\r\n");

    int touch_num = get_touchscreen_event_num();
    if (touch_num != -1)
    {
        printf("get touch success: event%d\r\n", touch_num);
    }
    else
    {
        printf("get touch fail,exit\r\n");
        goto exit;
    }

exit:
    printf("==============exit=============\r\n");
    return 0;
}