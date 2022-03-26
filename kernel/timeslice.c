/*
 * @Author: luoqi 
 * @Date: 2021-04-29 19:27:09 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 16:16:53
 */

#include "timeslice.h"

static LIST_HEAD(timeslice_task_list);
static LIST_HEAD(timeslice_task_del_list);
static unsigned long timeslice_task_id = 0;

void timeslice_exec()
{
    ListObj* node;
    TimesilceTaskObj *task;

    list_for_each(node, &timeslice_task_list)
    {
        task = list_entry(node, TimesilceTaskObj, timeslice_task_node);
        if(task->is_run == TASK_RUN)
        {
            // func_start();
            task->task_hdl();
            task->is_run = TASK_STOP;
            // task->run_time = func_end();
        }
    }
}

void timeslice_tick()
{
    ListObj* node;
    TimesilceTaskObj *task;

    list_for_each(node, &timeslice_task_list)
    {
        task = list_entry(node, TimesilceTaskObj, timeslice_task_node);
        if(task->timer != 0)
        {
            task->timer --;
            if(task->timer == 0)
            {
                task->is_run = TASK_RUN;
                task->timer = task->timeslice_len;
            }
        }
    }
}

unsigned int timeslice_task_num_get()
{
    return list_len(&timeslice_task_list);
}

void timeslice_task_init(TimesilceTaskObj* task, 
                         const char* name,
                         void (*task_hdl)(void),
                         unsigned int timeslice_len, 
                         const char* usage)
{
    timeslice_task_id ++;
    task->name = name;
    task->id = timeslice_task_id;
    task->is_run = TASK_STOP;
    task->task_hdl = task_hdl;
    task->timer = timeslice_len;
    task->timeslice_len = timeslice_len;
    task->usage = usage;
    task->run_time = 0;
}

int timeslice_task_add(TimesilceTaskObj* task)
{
    if(timeslice_del_task_isexist(task) == 1)
    {
        task->is_run = TASK_STOP;
        list_remove(&task->timeslice_task_node);
    }

    if(timeslice_task_isexist(task) == 0)
    {
        list_insert_before(&timeslice_task_list, &task->timeslice_task_node);
        return 0;
    }
    return 1;
}

int timeslice_task_del(TimesilceTaskObj* task)
{
    if(timeslice_task_isexist(task) == 1)
    {
        task->is_run = TASK_STOP;
        list_remove(&task->timeslice_task_node);
    }

    if(timeslice_del_task_isexist(task) == 0)
    {
        list_insert_before(&timeslice_task_del_list, &task->timeslice_task_node);
        return 0;
    }
    else 
        return 1;
}


int timeslice_task_isexist(TimesilceTaskObj* task)
{
    ListObj* node;
    TimesilceTaskObj *_task;

    list_for_each(node, &timeslice_task_list)
    {
        _task = list_entry(node, TimesilceTaskObj, timeslice_task_node);
        if(task->id == _task->id)
            return 1;
        else
            continue;
    }
    return 0;
}

int timeslice_del_task_isexist(TimesilceTaskObj* task)
{
    ListObj* node;
    TimesilceTaskObj *_task;

    list_for_each(node, &timeslice_task_del_list)
    {
        _task = list_entry(node, TimesilceTaskObj, timeslice_task_node);
        if(task->id == _task->id)
            return 1;
        else
            continue;
    }
    return 0;
}

unsigned int timeslice_slice_len_get(TimesilceTaskObj* task)
{
    return task->timeslice_len;
}

void timeslice_slice_change(TimesilceTaskObj* obj, unsigned int slice_len)
{
    obj->timeslice_len = slice_len;
}

TimesilceTaskObj* timeslice_obj_get(unsigned int task_id)
{
    ListObj* node = &timeslice_task_list;

    for(unsigned int i = task_id; i > 0; i--)
        node = node->next;

    return list_entry(node, TimesilceTaskObj, timeslice_task_node);
}

unsigned int timeslice_del_task_num_get()
{
    return list_len(&timeslice_task_del_list);
}

TimesilceTaskObj* timeslice_del_obj_get(unsigned int task_id)
{
    ListObj* node = &timeslice_task_del_list;

    for(unsigned int i = task_id; i > 0; i--)
        node = node->next;

    return list_entry(node, TimesilceTaskObj, timeslice_task_node);
}
