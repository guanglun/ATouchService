#ifndef __TOUCH_H_
#define __TOUCH_H_

#define TOUCH_NORMAL        0
#define TOUCH_MOVE_NORMAL   1
#define TOUCH_MOVE_UP       2

typedef struct struct_touch
{
    char is_use;
    char type;
    __s32 start_x;
    __s32 start_y;   
    __s32 end_x;
    __s32 end_y;
    __s32 now_x;
    __s32 now_y;
    int step;
    int step_count;
} s_touch;

int touch_init(void);
int touch_down(s_touch *touch);
int touch_move(int id,__s32 x, __s32 y);
int touch_up(int id);


#endif
