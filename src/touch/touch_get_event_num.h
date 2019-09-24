#ifndef __TOUCH_GET_EVENT_NUM_H_
#define __TOUCH_GET_EVENT_NUM_H_

#define TOUCH_TYPE_ERROR    0
#define TOUCH_TYPE_A        1
#define TOUCH_TYPE_B        2

int get_touchscreen_event_num(char *touch_type,int *size_x, int *size_y);



#endif
