/*
 * @Author: luoqi 
 * @Date: 2021-04-29 19:27:49 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-04-29 19:44:11
 */

#ifndef _TIMESLICE_H
#define _TIMESLICE_H

#include "./list.h"

typedef enum {
    TASK_STOP,
    TASK_RUN
} IsTaskRun;

typedef struct timesilce
{
    unsigned int id;
    void (*task_hdl)(void);
    IsTaskRun is_run;
    unsigned int timer;
    unsigned int timeslice_len;
    ListObj timeslice_task_list;
} TimesilceTaskObj;

void timeslice_exec(void);
void timeslice_tick(void);
void timeslice_task_init(TimesilceTaskObj* obj, void (*task_hdl)(void), unsigned int id, unsigned int timeslice_len);
void timeslice_task_add(TimesilceTaskObj* obj);
void timeslice_task_del(TimesilceTaskObj* obj);
unsigned int timeslice_get_task_timeslice_len(TimesilceTaskObj* obj);
unsigned int timeslice_get_task_num(void);
unsigned char timeslice_task_isexist(TimesilceTaskObj* obj);

#endif
