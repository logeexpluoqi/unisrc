/*
 * @Author: luoqi 
 * @Date: 2021-04-29 19:27:49 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-05-18 16:20:14
 */

#ifndef _TIMESLICE_H
#define _TIMESLICE_H

#include "../common/list.h"

typedef enum {
    TASK_STOP,
    TASK_RUN
} IsTaskRun;

typedef struct timesilce
{
    const char* name;
    unsigned int id;
    void (*task_hdl)(void);
    IsTaskRun is_run;
    unsigned int timer;
    unsigned int timeslice_len;
    ListObj timeslice_task_list;
    const char* usage;
} TimesilceTaskObj;

void timeslice_exec(void);
void timeslice_tick(void);
void timeslice_task_init(TimesilceTaskObj* obj, 
                         const char* name, 
                         void (*task_hdl)(void),
                         unsigned int timeslice_len, 
                         const char* usage);
void timeslice_task_add(TimesilceTaskObj* obj);
unsigned char timeslice_task_del(TimesilceTaskObj* obj);
void timeslice_change_slice(TimesilceTaskObj* obj, unsigned int slice_len);
unsigned int timeslice_get_task_timeslice_len(TimesilceTaskObj* obj);
unsigned int timeslice_get_task_num(void);
unsigned char timeslice_task_isexist(TimesilceTaskObj* obj);
TimesilceTaskObj* timeslice_obj_get(unsigned int serial);
unsigned int timeslice_get_del_task_num(void);
TimesilceTaskObj* timeslice_del_obj_get(unsigned int serial);

#endif
