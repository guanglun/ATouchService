#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "proc_mouse.h"
#include "proc_keyboard.h"
#include "list_keyboard_event.h"
#include "touch.h"
#include "pubg.h"

#define MOVE_STEP 150

KBEventNode *headNode;
KBEventNode *headNodeFlag;

int move_x, move_y;
void set_move_xy(int x,int y)
{
    move_x = x;
    move_y = y;
}

void kb_get_event(char *buf, int len)
{
    KBEventNode *event;
    KBEventNode *pNode;
    
    static char is_first = 1;
    int i = 0;

    if (is_first == 1)
    {
        is_first = 0;

        kb_list_init(&headNode);
        kb_list_init(&headNodeFlag);
    }

    if ((buf[0] & 0x01) == 0x01)
    {
        event = kb_list_get_node(headNode, KB_LEFT_CTRL);
        if (event != NULL)
        {
            if (event->event == EVENT_K_DOWN)
            {
                event->event = EVENT_K_KEEP;
            }
            else if (event->event == EVENT_K_UP)
            {
                event->event = EVENT_K_DOWN;
            }
        }
        else
        {
            kb_list_insert(headNode, KB_LEFT_CTRL, EVENT_K_DOWN);
        }
        kb_list_insert(headNodeFlag, KB_LEFT_CTRL, EVENT_K_DOWN);
    }

    if ((buf[0] & 0x02) == 0x02)
    {
        event = kb_list_get_node(headNode, KB_LEFT_SHIFT);
        if (event != NULL)
        {
            if (event->event == EVENT_K_DOWN)
            {
                event->event = EVENT_K_KEEP;
            }
            else if (event->event == EVENT_K_UP)
            {
                event->event = EVENT_K_DOWN;
            }
        }
        else
        {
            kb_list_insert(headNode, KB_LEFT_SHIFT, EVENT_K_DOWN);
        }
        kb_list_insert(headNodeFlag, KB_LEFT_SHIFT, EVENT_K_DOWN);
    }

    if ((buf[0] & 0x04) == 0x04)
    {
        event = kb_list_get_node(headNode, KB_LEFT_ALT);
        if (event != NULL)
        {
            if (event->event == EVENT_K_DOWN)
            {
                event->event = EVENT_K_KEEP;
            }
            else if (event->event == EVENT_K_UP)
            {
                event->event = EVENT_K_DOWN;
            }
        }
        else
        {
            kb_list_insert(headNode, KB_LEFT_ALT, EVENT_K_DOWN);
        }
        kb_list_insert(headNodeFlag, KB_LEFT_ALT, EVENT_K_DOWN);
    }

    if ((buf[0] & 0x08) == 0x08)
    {
        event = kb_list_get_node(headNode, KB_LEFT_GUI);
        if (event != NULL)
        {
            if (event->event == EVENT_K_DOWN)
            {
                event->event = EVENT_K_KEEP;
            }
            else if (event->event == EVENT_K_UP)
            {
                event->event = EVENT_K_DOWN;
            }
        }
        else
        {
            kb_list_insert(headNode, KB_LEFT_GUI, EVENT_K_DOWN);
        }
        kb_list_insert(headNodeFlag, KB_LEFT_GUI, EVENT_K_DOWN);
    }

    if ((buf[0] & 0x10) == 0x10)
    {
        event = kb_list_get_node(headNode, KB_RIGHT_CTRL);
        if (event != NULL)
        {
            if (event->event == EVENT_K_DOWN)
            {
                event->event = EVENT_K_KEEP;
            }
            else if (event->event == EVENT_K_UP)
            {
                event->event = EVENT_K_DOWN;
            }
        }
        else
        {
            kb_list_insert(headNode, KB_RIGHT_CTRL, EVENT_K_DOWN);
        }
        kb_list_insert(headNodeFlag, KB_RIGHT_CTRL, EVENT_K_DOWN);
    }

    if ((buf[0] & 0x20) == 0x20)
    {
        event = kb_list_get_node(headNode, KB_RIGHT_SHIFT);
        if (event != NULL)
        {
            if (event->event == EVENT_K_DOWN)
            {
                event->event = EVENT_K_KEEP;
            }
            else if (event->event == EVENT_K_UP)
            {
                event->event = EVENT_K_DOWN;
            }
        }
        else
        {
            kb_list_insert(headNode, KB_RIGHT_SHIFT, EVENT_K_DOWN);
        }
        kb_list_insert(headNodeFlag, KB_RIGHT_SHIFT, EVENT_K_DOWN);
    }

    if ((buf[0] & 0x40) == 0x40)
    {
        event = kb_list_get_node(headNode, KB_RIGHT_ALT);
        if (event != NULL)
        {
            if (event->event == EVENT_K_DOWN)
            {
                event->event = EVENT_K_KEEP;
            }
            else if (event->event == EVENT_K_UP)
            {
                event->event = EVENT_K_DOWN;
            }
        }
        else
        {
            kb_list_insert(headNode, KB_RIGHT_ALT, EVENT_K_DOWN);
        }
        kb_list_insert(headNodeFlag, KB_RIGHT_ALT, EVENT_K_DOWN);
    }

    if ((buf[0] & 0x80) == 0x80)
    {
        event = kb_list_get_node(headNode, KB_RIGHT_GUI);
        if (event != NULL)
        {
            if (event->event == EVENT_K_DOWN)
            {
                event->event = EVENT_K_KEEP;
            }
            else if (event->event == EVENT_K_UP)
            {
                event->event = EVENT_K_DOWN;
            }
        }
        else
        {
            kb_list_insert(headNode, KB_RIGHT_GUI, EVENT_K_DOWN);
        }
        kb_list_insert(headNodeFlag, KB_RIGHT_GUI, EVENT_K_DOWN);
    }

    for (i = 2; i < 8; i++)
    {
        if (buf[i] != 0x00)
        {
            event = kb_list_get_node(headNode, buf[i]);
            if (event != NULL)
            {
                if (event->event == EVENT_K_DOWN)
                {
                    event->event = EVENT_K_KEEP;
                }
                else if (event->event == EVENT_K_UP)
                {
                    event->event = EVENT_K_DOWN;
                }
            }
            else
            {
                kb_list_insert(headNode, buf[i], EVENT_K_DOWN);
            }
            kb_list_insert(headNodeFlag, buf[i], EVENT_K_DOWN);
        }
    }
    pNode = headNode->next;
    while (pNode != NULL)
    {
        event = kb_list_get_node(headNodeFlag, pNode->code);
        if (event == NULL)
        {
            pNode->event = EVENT_K_UP;
        }
        pNode = pNode->next;
    }
    kb_list_clear(headNodeFlag);
}

void proc_keyboard(char *buf, int len)
{
    
    static char is_left_shift_down = 0;

    static int te_move = -1,te_jump = -1,te_squat = -1,te_lie = -1,
            te_package = -1,te_arms_left = -1,te_arms_right = -1,te_map = -1,
            te_aim = -1,te_check_package = -1,te_door = -1,te_drive = -1,te_getoff = -1,te_grenade = -1,
            te_medicine = -1,te_reload = -1,te_save = -1,te_sprint = -1,te_follow = -1,te_pick = -1,
            te_ride = -1,te_pick1 = -1,te_pick2 = -1,te_pick3 = -1;

    KBEventNode *pNode;
    KBEventNode *pNodeLast;
    s_touch touch;

    kb_get_event(buf, len);
    kb_list_printf(headNode);

    pNode = headNode->next;
    while (pNode != NULL)
    {
        /**移动部分 start**/
        //Log.i("DEBUG0", "Event:" + pNode->event + " Code:" + pNode->code);

        if (pNode->code == KB_W)
        {
            if (pNode->event == EVENT_K_DOWN)
            {
                move_y -= MOVE_STEP;
            }
            else if (pNode->event == EVENT_K_UP)
            {
                move_y += MOVE_STEP;
            }
        }

        if (pNode->code == KB_S)
        {
            if (pNode->event == EVENT_K_DOWN)
            {
                move_y += MOVE_STEP;
            }
            else if (pNode->event == EVENT_K_UP)
            {
                move_y -= MOVE_STEP;
            }
        }

        if (pNode->code == KB_A)
        {
            if (pNode->event == EVENT_K_DOWN)
            {
                move_x -= MOVE_STEP;
            }
            else if (pNode->event == EVENT_K_UP)
            {
                move_x += MOVE_STEP;
            }
        }

        if (pNode->code == KB_D)
        {
            if (pNode->event == EVENT_K_DOWN)
            {
                move_x += MOVE_STEP;
            }
            else if (pNode->event == EVENT_K_UP)
            {
                move_x -= MOVE_STEP;
            }
        }

        if (pNode->code == KB_LEFT_SHIFT)
        {
            if (pNode->event == EVENT_K_DOWN)
            {
                is_left_shift_down = 1;
            }
            else if (pNode->event == EVENT_K_UP)
            {
                is_left_shift_down = 0;
            }
        }
        /**移动部分 end**/

        if (pNode->code == KB_1)
        {
            if (pNode->event == EVENT_K_DOWN && te_pick1 == -1)
            {
                touch.start_x = s_pubg.N51_Pick1X;
                touch.start_y = s_pubg.N52_Pick1Y;
                touch.type = TOUCH_NORMAL;

                te_pick1 = touch_down(&touch);
            }
            else if (pNode->event == EVENT_K_UP && te_pick1 != -1)
            {
                touch_up(te_pick1);
                te_pick1 = -1;
            }
        }

        if (pNode->code == KB_2)
        {
            if (pNode->event == EVENT_K_DOWN && te_pick2 == -1)
            {
                touch.start_x = s_pubg.N53_Pick2X;
                touch.start_y = s_pubg.N54_Pick2Y;
                touch.type = TOUCH_NORMAL;

                te_pick2 = touch_down(&touch);                
            }
            else if (pNode->event == EVENT_K_UP && te_pick2 != -1)
            {
                touch_up(te_pick2);
                te_pick2 = -1;
            }
        }

        if (pNode->code == KB_3)
        {
            if (pNode->event == EVENT_K_DOWN && te_pick3 == -1)
            {
                touch.start_x = s_pubg.N55_Pick3X;
                touch.start_y = s_pubg.N56_Pick3Y;
                touch.type = TOUCH_NORMAL;

                te_pick3 = touch_down(&touch);                    
                
            }
            else if (pNode->event == EVENT_K_UP && te_pick3 != -1)
            {
                touch_up(te_pick3);
                te_pick3 = -1;
            }
        }

        if (pNode->code == KB_Q)
        {
            if (pNode->event == EVENT_K_DOWN && te_arms_left == -1)
            {
                touch.start_x = s_pubg.N19_ArmsLeftX;
                touch.start_y = s_pubg.N20_ArmsLeftY;
                touch.type = TOUCH_NORMAL;

                te_arms_left = touch_down(&touch);                  
                
            }
            else if (pNode->event == EVENT_K_UP && te_arms_left != -1)
            {
                touch_up(te_arms_left);
                te_arms_left = -1;
            }
        }

        if (pNode->code == KB_E)
        {
            if (pNode->event == EVENT_K_DOWN && te_arms_right == -1)
            {
                touch.start_x = s_pubg.N21_ArmsRightX;
                touch.start_y = s_pubg.N22_ArmsRightY;
                touch.type = TOUCH_NORMAL;

                te_arms_right = touch_down(&touch);     
        
            }
            else if (pNode->event == EVENT_K_UP && te_arms_right != -1)
            {
                touch_up(te_arms_right);
                te_arms_right = -1;
            }
        }

        if (pNode->code == KB_SPACE)
        {
            if (pNode->event == EVENT_K_DOWN && te_jump == -1)
            {
                touch.start_x = s_pubg.N7_JumpX;
                touch.start_y = s_pubg.N8_JumpY;
                touch.type = TOUCH_NORMAL;

                te_jump = touch_down(&touch);                    
                
            }
            else if (pNode->event == EVENT_K_UP && te_jump != -1)
            {
                touch_up(te_jump);
                te_jump = -1;
            }
        }

        if (pNode->code == KB_LEFT_ALT)
        {
            if (pNode->event == EVENT_K_DOWN && te_squat == -1)
            {
                touch.start_x = s_pubg.N9_SquatX;
                touch.start_y = s_pubg.N10_SquatY;
                touch.type = TOUCH_NORMAL;

                te_squat = touch_down(&touch);                   
                
            }
            else if (pNode->event == EVENT_K_UP && te_squat != -1)
            {
                touch_up(te_squat);
                te_squat = -1;
            }
        }

        if (pNode->code == KB_LEFT_CTRL)
        {
            if (pNode->event == EVENT_K_DOWN && te_lie == -1)
            {
                touch.start_x = s_pubg.N11_LieX;
                touch.start_y = s_pubg.N12_LieY;
                touch.type = TOUCH_NORMAL;

                te_lie = touch_down(&touch);                   
                
            }
            else if (pNode->event == EVENT_K_UP && te_lie != -1)
            {
                touch_up(te_lie);
                te_lie = -1;
            }
        }

        if (pNode->code == KB_B)
        {
            if (pNode->event == EVENT_K_DOWN && te_package == -1)
            {
                touch.start_x = s_pubg.N17_PackageX;
                touch.start_y = s_pubg.N18_PackageY;
                touch.type = TOUCH_NORMAL;

                te_package = touch_down(&touch);                   
            
            }
            else if (pNode->event == EVENT_K_UP && te_package != -1)
            {
                touch_up(te_package);
                te_package = -1;
            }
        }

        if (pNode->code == KB_M)
        {
            if (pNode->event == EVENT_K_DOWN && te_map == -1)
            {
                touch.start_x = s_pubg.N23_MapX;
                touch.start_y = s_pubg.N24_MapY;
                touch.type = TOUCH_NORMAL;

                te_map = touch_down(&touch);    
                
            }
            else if (pNode->event == EVENT_K_UP && te_map != -1)
            {
                touch_up(te_map);
                te_map = -1;
            }
        }

        if (pNode->code == KB_R)
        {
            if (pNode->event == EVENT_K_DOWN && te_reload == -1)
            {
                touch.start_x = s_pubg.N39_ReloadX;
                touch.start_y = s_pubg.N40_ReloadY;
                touch.type = TOUCH_NORMAL;

                te_reload = touch_down(&touch);                   
               
            }
            else if (pNode->event == EVENT_K_UP && te_reload != -1)
            {
                touch_up(te_reload);
                te_reload = -1;
            }
        }

        if (pNode->code == KB_Z)
        {
            if (pNode->event == EVENT_K_DOWN && te_drive == -1)
            {
                touch.start_x = s_pubg.N31_DriveX;
                touch.start_y = s_pubg.N32_DriveY;
                touch.type = TOUCH_NORMAL;

                te_drive = touch_down(&touch);                   
                
            }
            else if (pNode->event == EVENT_K_UP && te_drive != -1)
            {
                touch_up(te_drive);
                te_drive = -1;
            }
        }

        if (pNode->code == KB_X)
        {
            if (pNode->event == EVENT_K_DOWN && te_ride == -1)
            {
                touch.start_x = s_pubg.N49_RideX;
                touch.start_y = s_pubg.N50_RideY;
                touch.type = TOUCH_NORMAL;

                te_ride = touch_down(&touch);                     
                
            }
            else if (pNode->event == EVENT_K_UP && te_ride != -1)
            {
                touch_up(te_ride);
                te_ride = -1;
            }
        }

        if (pNode->code == KB_C)
        {
            if (pNode->event == EVENT_K_DOWN && te_getoff == -1)
            {
                touch.start_x = s_pubg.N33_GetOffX;
                touch.start_y = s_pubg.N34_GetOffY;
                touch.type = TOUCH_NORMAL;

                te_getoff = touch_down(&touch);                     
                
            }
            else if (pNode->event == EVENT_K_UP && te_getoff != -1)
            {
                touch_up(te_getoff);
                te_getoff = -1;
            }
        }

        if (pNode->code == KB_T)
        {
            if (pNode->event == EVENT_K_DOWN && te_check_package == -1)
            {
                touch.start_x = s_pubg.N27_CheckPackageX;
                touch.start_y = s_pubg.N28_CheckPackageY;
                touch.type = TOUCH_NORMAL;

                te_check_package = touch_down(&touch);                       
               
            }
            else if (pNode->event == EVENT_K_UP && te_check_package != -1)
            {
                touch_up(te_check_package);
                te_check_package = -1;
            }
        }

        if (pNode->code == KB_H)
        {
            if (pNode->event == EVENT_K_DOWN && te_save == -1)
            {
                touch.start_x = s_pubg.N41_SaveX;
                touch.start_y = s_pubg.N42_SaveY;
                touch.type = TOUCH_NORMAL;

                te_save = touch_down(&touch);                   
                
            }
            else if (pNode->event == EVENT_K_UP && te_save != -1)
            {
                touch_up(te_save);
                te_save = -1;
            }
        }

        if (pNode->code == KB_G)
        {
            if (pNode->event == EVENT_K_DOWN && te_medicine == -1)
            {
                touch.start_x = s_pubg.N37_MedicineX;
                touch.start_y = s_pubg.N38_MedicineY;
                touch.type = TOUCH_NORMAL;

                te_medicine = touch_down(&touch);  
                
            }
            else if (pNode->event == EVENT_K_UP && te_medicine != -1)
            {
                touch_up(te_medicine);
                te_medicine = -1;
            }
        }

        if (pNode->code == KB_F)
        {
            if (pNode->event == EVENT_K_DOWN)
            {
                set_watch_status(1);
            }
            else if (pNode->event == EVENT_K_UP)
            {
                set_watch_status(0);
            }
        }
        
        pNode = pNode->next;
    }

    /**移动部分 start**/

    //Log.i("DEBUG2", "MouseX:" + move_x + " MouseY:" + move_y);
    if ((move_x != s_pubg.N5_MoveX || move_y != s_pubg.N6_MoveY) && te_move == -1)
    {
        if (is_left_shift_down && move_y == s_pubg.N6_MoveY - MOVE_STEP && move_x == s_pubg.N5_MoveX) //shift
        {
            touch.start_x = s_pubg.N5_MoveX;
            touch.start_y = s_pubg.N6_MoveY;
            touch.type = TOUCH_MOVE_NORMAL;
            touch.end_x = s_pubg.N43_SprintX;
            touch.end_y = s_pubg.N44_SprintY;
            touch.step = 2;
        }
        else
        {
            touch.start_x = s_pubg.N5_MoveX;
            touch.start_y = s_pubg.N6_MoveY;
            touch.type = TOUCH_MOVE_NORMAL;
            touch.end_x = move_x;
            touch.end_y = move_y;
            touch.step = 2;
        }
        
        te_move = touch_down(&touch); 
    }
    else if (move_x == s_pubg.N5_MoveX && move_y == s_pubg.N6_MoveY && te_move != -1)
    {
        touch_move(te_move,s_pubg.N5_MoveX, s_pubg.N6_MoveY - MOVE_STEP);

        usleep(60);

        touch_up(te_move);
        te_move = -1;
    }
    else if (te_move != -1)
    {
        if (is_left_shift_down && move_y == s_pubg.N6_MoveY - MOVE_STEP && move_x == s_pubg.N5_MoveX) //shift
        {
            touch_move(te_move,s_pubg.N43_SprintX, s_pubg.N44_SprintY);

        }
        else
        {
            touch_move(te_move,move_x, move_y);
        }
    }
    /**移动部分 end**/


    pNodeLast = headNode;
    pNode = headNode->next;
    
    while (pNode != NULL)
    {
        if(pNode->event == EVENT_K_UP)
        {
            pNodeLast->next = pNode->next;
            kb_list_delete_node(headNode,pNode->code);
            pNode = pNodeLast->next;
        }else{
            pNodeLast = pNode;
            pNode = pNode->next;
        }
    }
}
