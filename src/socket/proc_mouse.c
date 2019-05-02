#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "proc_mouse.h"
#include "proc_keyboard.h"
#include "pubg.h"
#include "touch.h"

#define FACE_STEP 400
#define WATCH_STEP 400

char is_watch = 0;
int te_face = -1;
unsigned short face_x = 0, face_y = 0;

void proc_mouse_reset(void)
{
    if(te_face != -1)
    {
        touch_up(te_face);
        te_face = -1;
    }
}

void proc_mouse(char *buf, int len)
{
    static int te_attack = -1,te_aim = -1;
    
    s_touch touch;
    char temp[200],temp2[10];;
    int temp_len;

    if ((te_face == -1) && ((buf[0] & 0x04) == 0x04))
    {

        if (is_watch == 0)
        {
            face_x = s_pubg.N13_FaceX;
            face_y = s_pubg.N14_FaceY;            
            touch.start_x = s_pubg.N13_FaceX;
            touch.start_y = s_pubg.N14_FaceY;
            touch.type = TOUCH_NORMAL;
        }
        else
        {
            face_x = s_pubg.N15_WatchX;
            face_y = s_pubg.N16_WatchY;               
            touch.start_x = s_pubg.N15_WatchX;
            touch.start_y = s_pubg.N16_WatchY;
            touch.type = TOUCH_NORMAL;
        }

        te_face = touch_down(&touch);

        temp2[0] = 0x00;
        temp_len = cmd_atouch_creat(0x02, temp2, 1, temp);
        socket_atouch_send(temp,temp_len);
    }
    else if ((te_face != -1) && ((buf[0] & 0x04) == 0x04))
    {

        touch_up(te_face);
        te_face = -1;

        temp2[0] = 0x01;
        temp_len = cmd_atouch_creat(0x02, temp2, 1, temp);
        socket_atouch_send(temp,temp_len);
    }

    if (te_face != -1)
    {

        face_x += (signed char)buf[1];
        face_y += (signed char)buf[2];

        //LOG("%d\t%d\t%d\t%d\r\n",face_x,face_y,(signed char)buf[1],(signed char)buf[2]);

        if (is_watch == 0)
        {
            if ( (face_x < (s_pubg.N13_FaceX - FACE_STEP)) || (face_x > (s_pubg.N13_FaceX + FACE_STEP)) ||
                (face_y < (s_pubg.N14_FaceY - FACE_STEP)) || (face_y > (s_pubg.N14_FaceY + FACE_STEP)))
            {
                touch_up(te_face);
                te_face = -1;
                face_x = s_pubg.N13_FaceX;
                face_y = s_pubg.N14_FaceY;
                face_x += buf[1];
                face_y += buf[2];
                touch.start_x = s_pubg.N13_FaceX;
                touch.start_y = s_pubg.N14_FaceY;
                touch.type = TOUCH_NORMAL;
                
                te_face= touch_down(&touch);
            }
        }
        else
        {

            face_x = limit(face_x, s_pubg.N15_WatchX - WATCH_STEP*2, s_pubg.N15_WatchX + WATCH_STEP);
            face_y = limit(face_y, s_pubg.N16_WatchY - WATCH_STEP, s_pubg.N16_WatchY + WATCH_STEP);
        }

        touch_move(te_face,face_x, face_y);

        if ((te_attack == -1) && ((buf[0] & 0x01) == 0x01))
        {
            touch.start_x = s_pubg.N3_AttackX;
            touch.start_y = s_pubg.N4_AttackY;
            touch.type = TOUCH_NORMAL;

            te_attack = touch_down(&touch);
            
        }
        else if ((te_attack != -1) && ((buf[0] & 0x01) == 0x00))
        {

            touch_up(te_attack);
            te_attack = -1;
        }

        if ((te_aim == -1) && ((buf[0] & 0x02) == 0x02))
        {
            touch.start_x = s_pubg.N25_AimX;
            touch.start_y = s_pubg.N26_AimY;
            touch.type = TOUCH_NORMAL;

            te_aim = touch_down(&touch);
            
        }
        else if ((te_aim != -1) && ((buf[0] & 0x02) == 0x00))
        {
            touch_up(te_aim);
            te_aim = -1;
        }
    }
    else
    {
        temp_len = cmd_atouch_creat(0x01, buf, 4, temp);
        socket_atouch_send(temp,temp_len);
    }
}

void set_watch_status(char isdown)
{
    s_touch touch;
    is_watch = isdown;
    if (is_watch && te_face != -1)
    {
        touch_up(te_face);
        usleep(60000);

        face_x = s_pubg.N15_WatchX;
        face_y = s_pubg.N16_WatchY;

        touch.start_x = s_pubg.N15_WatchX;
        touch.start_y = s_pubg.N16_WatchY;
        touch.type = TOUCH_NORMAL;

        te_face= touch_down(&touch);
    }
    else if (is_watch == 0 && te_face != -1)
    {
        touch_up(te_face);
        usleep(60000);
        face_x = s_pubg.N13_FaceX;
        face_y = s_pubg.N14_FaceY;

        touch.start_x = s_pubg.N13_FaceX;
        touch.start_y = s_pubg.N14_FaceY;
        touch.type = TOUCH_NORMAL;

        te_face= touch_down(&touch);
    }
}