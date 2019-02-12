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
#include "proc_mouse.h"
#include "proc_keyboard.h"

void adb_receive_protocol(char *buf, int len)
{
    char temp[200];
    int temp_len = 0;
    switch (buf[0])
    {
    case 0x00:

        temp_len = cmd_atouch_creat(0x00, buf + 1, 3, temp);
        socket_atouch_send(temp,temp_len);

        break;
    case 0x02:
        if (len >= 4)
        {
            proc_mouse(buf+1, len-1);
        }
        break;

    case 0x03:
        if (len == 9)
        {
            proc_keyboard(buf+1, len-1);
        }
        break;

    default:
        break;
    }
}

int adb_receive(char *buf, int len)
{
    static int receive_adb_flag = 0, data_adb_len = 0;
    static char data_adb_buffer[2048];
    static char check_adb = 0;

    char isSuccess = 0;
    int i = 0;

    for (; i < len; i++)
    {
        if (receive_adb_flag == 0 && buf[i] == 0xAA)
        {
            receive_adb_flag = 1;
        }
        else if (receive_adb_flag == 1 && buf[i] == 0xBB)
        {
            data_adb_len = 0;
            check_adb = 0;
            receive_adb_flag = 2;
        }
        else if (receive_adb_flag == 2)
        {
            data_adb_len = buf[i];
            receive_adb_flag = 3;
            check_adb += buf[i];
        }
        else if ((receive_adb_flag >= 3) && (receive_adb_flag < (data_adb_len + 3)))
        {
            data_adb_buffer[receive_adb_flag - 3] = buf[i];
            receive_adb_flag++;
            check_adb += buf[i];
        }
        else if ((receive_adb_flag == (data_adb_len + 3)) && (check_adb == buf[i]))
        {
            isSuccess = 1;
            log_byte(data_adb_buffer, data_adb_len);
            adb_receive_protocol(data_adb_buffer, data_adb_len);
            receive_adb_flag = 0;
        }
        else if (buf[i] == 0xAA)
        {
            receive_adb_flag = 1;
        }
        else
        {
            receive_adb_flag = 0;
        }
    }

    return isSuccess;
}

