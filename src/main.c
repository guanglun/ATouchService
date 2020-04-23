#include <stdio.h>


#include "touch.h"
#include "log.h"
#include "server.h"
#include "list_keyboard_event.h"

#include "version.h"

KBEventNode *headNode;

int main()
{
    

    LOG("\r\n==============start=============\r\n");\
    LOG("\r\n==============%s=============\r\n",VERSION);
    LOG("ATouchService is runing\r\n");

    check_run();

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
    // touch.end_y = 500;
    // touch.step = 50;
    // touch.type =  TOUCH_MOVE_NORMAL;

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

    // touch_up(touch1);

    
    // if(kb_list_init(&headNode) == -1)
    // {
    //     LOG("kb_list_init Fail\r\n");
    // }else{
    //     LOG("kb_list_init Success\r\n");
    // }
    // LOG("ADDR:%08X\r\n",headNode);
    // kb_list_insert(headNode,1,1);
    // kb_list_insert(headNode,2,2);
    // LOG("p1:\r\n");
    // kb_list_printf(headNode);
    // kb_list_clear(headNode);
    // LOG("p2:\r\n");
    // kb_list_printf(headNode);

    while(1)
    {
        sleep(1000);
    }

exit:
    LOG("==============exit=============\r\n");
    return 0;
}