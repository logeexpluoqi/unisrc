/*
 * @Author: luoqi 
 * @Date: 2021-04-29 19:27:49 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-04-29 19:44:11
 */

#ifndef _TIMESLICE_H
#define _TIMESLICE_H

#include "../common/list.h"

typedef enum {
    TASK_STOP,
    TASK_RUNNING
} IsTaskRun;

typedef struct timesilce
{
    void (*task_hdl)(void);
    IsTaskRun is_running;
    unsigned int timer;
    unsigned int timer_init_val;
} TimesilceTaskObj;

void timeslice_init(void);
void timeslice_exec(void);
void timeslice_tick(void);
void timeslice_task_add(TimesilceTaskObj* obj);
void timeslice_task_del(TimesilceTaskObj* obj);
unsigned char timeslice_task_isexist(TimesilceTaskObj* obj);

#endif
