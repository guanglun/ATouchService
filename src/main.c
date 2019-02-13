#include <stdio.h>


#include "touch.h"
#include "log.h"
#include "server.h"
#include "list_keyboard_event.h"

KBEventNode *headNode;

int main()
{
    

    LOG("\r\n==============start=============\r\n");
    printf("ATouchService is runing\r\n");

    

    if(touch_init() == -1)
    {
        goto exit;
    }

    if(server_init() == -1)
    {
        goto exit;
    }


    // s_touch touch;
    // touch.start_x = 100;
    // touch.start_y = 100;
    // touch.end_x = 500;
    // touch.end_y = 100;
    // touch.step = 10;
    // touch.type = TOUCH_MOVE_UP;

    // int touch1 = touch_down(&touch);

    // touch.start_x = 100;
    // touch.start_y = 150;
    // touch.end_x = 500;
    // touch.end_y = 150;
    // touch.step = 50;
    // int touch2 = touch_down(&touch);

    // touch.start_x = 100;
    // touch.start_y = 200;
    // touch.end_x = 500;
    // touch.end_y = 200;
    // touch.step = 100;
    // int touch3 = touch_down(&touch);

    //touch_up(touch1);

    
    // if(kb_list_init(&headNode) == -1)
    // {
    //     LOG("kb_list_init Fail\r\n");
    // }else{
    //     LOG("kb_list_init Success\r\n");
    // }
    // printf("ADDR:%08X\r\n",headNode);
    // kb_list_insert(headNode,1,1);
    // kb_list_insert(headNode,2,2);
    // printf("p1:\r\n");
    // kb_list_printf(headNode);
    // kb_list_clear(headNode);
    // printf("p2:\r\n");
    // kb_list_printf(headNode);

    while(1)
    {
        sleep(1);
    }

exit:
    LOG("==============exit=============\r\n");
    return 0;
}