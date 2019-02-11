#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#include "log.h"
#include "protocol_adb.h"
#include "protocol_atouch.h"
#include "touch.h"

void get_soft_mouse_buffer(char *buf)
{
    static int te_mouse = -1;
    unsigned short x = char2short(buf[1], buf[2]);
    unsigned short y = char2short(buf[3], buf[4]);

    if (((buf[0] & 0x01) == 0x01) && te_mouse == -1)
    {
        s_touch touch;
        touch.start_x = x;
        touch.start_y = y;
        touch.type = TOUCH_NORMAL;

        te_mouse = touch_down(&touch);
        
    }
    else if ((te_mouse != -1) && ((buf[0] & 0x01) == 0x00))
    {

        touch_up(te_mouse);
        te_mouse = -1;
    }
    else if ((te_mouse != -1) && ((buf[0] & 0x01) == 0x01))
    {
        touch_move(te_mouse,x,y);
    }


}

void atouch_receive_protocol(char *buf, int len)
{

    switch (buf[0])
    {
    case 0x01:
        LOG("receive keymap\r\n");
        get_pubg_buffer(buf + 1);

        break;
    case 0x02:
        get_soft_mouse_buffer(buf + 1);

        break;

    default:
        break;
    }
}

int atouch_receive(char *buf, int len)
{
    static int  receive_atouch_flag = 0, data_atouch_len = 0;
    static char data_atouch_buffer[2048];
    static char check_atouch = 0;

    char isSuccess = 0;
    int i = 0;
    for (; i < len; i++)
    {
        if (receive_atouch_flag == 0 && buf[i] == 0xCC)
        {
            receive_atouch_flag = 1;
        }
        else if (receive_atouch_flag == 1 && buf[i] == 0xDD)
        {
            data_atouch_len = 0;
            check_atouch = 0;
            receive_atouch_flag = 2;
        }
        else if (receive_atouch_flag == 2)
        {
            data_atouch_len = ((buf[i] << 8) & 0xffff);

            receive_atouch_flag = 3;
            check_atouch += buf[i];
        }
        else if (receive_atouch_flag == 3)
        {

            data_atouch_len |= (buf[i] & 0x00ff);
            receive_atouch_flag = 4;
            check_atouch += buf[i];
        }
        else if ((receive_atouch_flag >= 4) && (receive_atouch_flag < (data_atouch_len + 4)))
        {

            data_atouch_buffer[receive_atouch_flag - 4] = buf[i];
            receive_atouch_flag++;
            check_atouch += buf[i];
        }
        else if ((receive_atouch_flag == (data_atouch_len + 4)) && (check_atouch == buf[i]))
        {
            isSuccess = 1;
            //log_byte(data_atouch_buffer, data_atouch_len);
            atouch_receive_protocol(data_atouch_buffer, data_atouch_len);
            receive_atouch_flag = 0;
        }
        else if (buf[i] == 0xCC)
        {
            receive_atouch_flag = 1;
        }
        else
        {
            receive_atouch_flag = 0;
        }
    }

    return isSuccess;
}

int cmd_atouch_creat(char cmd, char *in_buf, int len, char *out_buf)
{
    char check_sum = 0;
    int i = 0;

    out_buf[0] = 0xCC;
    out_buf[1] = 0xDD;
    out_buf[2] = ((len + 1) >> 8);
    out_buf[3] = ((len + 1));
    out_buf[4] = cmd;

    memcpy(out_buf+5, in_buf, len);

    for (; i < len + 3; i++)
    {
        check_sum += out_buf[i + 2];
    }

    out_buf[5 + len] = check_sum;

    return len + 6;
}