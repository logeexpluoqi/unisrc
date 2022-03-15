/*
 * @Author: luoqi 
 * @Date: 2021-04-29 19:27:49 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 16:19:43
 */

#ifndef _TIMESLICE_H
#define _TIMESLICE_H

#include "qlist.h"

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
    unsigned long long run_time;
    ListObj timeslice_task_node;
    const char* usage;
} TimesilceTaskObj;

void timeslice_exec(void);

void timeslice_tick(void);

void timeslice_task_init(TimesilceTaskObj* task, 
                         const char* name, 
                         void (*task_hdl)(void),
                         unsigned int timeslice_len, 
                         const char* usage);

void timeslice_task_add(TimesilceTaskObj* task);

int timeslice_task_del(TimesilceTaskObj* task);

void timeslice_slice_change(TimesilceTaskObj* task, unsigned int slice_len);

unsigned int timeslice_slice_len_get(TimesilceTaskObj* task);

unsigned int timeslice_task_num_get(void);

int timeslice_task_isexist(TimesilceTaskObj* task);

int timesilce_del_task_isexist(TimesilceTaskObj* task);

TimesilceTaskObj* timeslice_obj_get(unsigned int task_id);

unsigned int timeslice_del_task_num_get(void);

TimesilceTaskObj* timeslice_del_obj_get(unsigned int task_id);

#endif
