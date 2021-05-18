/*
 * @Author: luoqi 
 * @Date: 2021-04-29 19:27:09 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-04-29 19:27:43
 */

#include "timeslice.h"

static LIST_HEAD(timeslice_task_list);
static unsigned int timeslice_task_id = 0;

void timeslice_exec()
{
    ListObj* node;
    TimesilceTaskObj* task;

    list_for_each(node, &timeslice_task_list)
    {
           
        task = list_entry(node, TimesilceTaskObj, timeslice_task_list);
        if (task->is_run == TASK_RUN)
        {
            task->task_hdl();
            task->is_run = TASK_STOP;
        }
    }
}

void timeslice_tick()
{
    ListObj* node;
    TimesilceTaskObj* task;

    list_for_each(node, &timeslice_task_list)
    {
        task = list_entry(node, TimesilceTaskObj, timeslice_task_list);
        if (task->timer != 0)
        {
            task->timer--;
            if (task->timer == 0)
            {
                task->is_run = TASK_RUN;
                task->timer = task->timeslice_len;
            }
        }
    }
}

unsigned int timeslice_get_task_num()
{
    return list_len(&timeslice_task_list);
}

void timeslice_task_init(TimesilceTaskObj* obj, void (*task_hdl)(void), unsigned int timeslice_len)
{
    timeslice_task_id ++;
    obj->id = timeslice_task_id;
    obj->is_run = TASK_STOP;
    obj->task_hdl = task_hdl;
    obj->timer = timeslice_len;
    obj->timeslice_len = timeslice_len;
}

void timeslice_task_add(TimesilceTaskObj* obj)
{
    list_insert_before(&timeslice_task_list, &obj->timeslice_task_list);
}

unsigned char timeslice_task_del(TimesilceTaskObj* obj)
{
    if (timeslice_task_isexist(obj))
    {
        list_remove(&obj->timeslice_task_list);
        return 0;
    }
    else
        return 1;
}


unsigned char timeslice_task_isexist(TimesilceTaskObj* obj)
{
    unsigned char isexist = 0;
    ListObj* node;
    TimesilceTaskObj* task;

    list_for_each(node, &timeslice_task_list)
    {
        task = list_entry(node, TimesilceTaskObj, timeslice_task_list);
        if (obj->id == task->id)
            isexist = 1;
    }

    return isexist;
}

unsigned int timeslice_get_task_timeslice_len(TimesilceTaskObj* obj)
{
    return obj->timeslice_len;
}
