/*
 * @Author: luoqi 
 * @Date: 2021-04-29 19:27:09 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 18:43:45
 */

#include "qtask.h"

static LIST_HEAD(timeslice_task_list);
static LIST_HEAD(timeslice_task_del_list);
static unsigned long timeslice_task_id = 0;

void qtask_exec()
{
    ListObj *node, *_node;
    QTaskObj *task;

    list_for_each_safe(node, _node, &timeslice_task_list) {
        task = list_entry(node, QTaskObj, qtask_node);
        if(task->is_run == TASK_RUN) {
            // func_start();
            task->task_hdl();
            task->is_run = TASK_STOP;
            // task->run_time = func_end;
        }
    }
}

void qtask_tick()
{
    ListObj* node;
    QTaskObj *task;

    list_for_each(node, &timeslice_task_list) {
        task = list_entry(node, QTaskObj, qtask_node);
        if(task->timer != 0) {
            task->timer --;
            if(task->timer == 0) {
                task->is_run = TASK_RUN;
                task->timer = task->timeslice;
            }
        }
    }
}

unsigned int qtask_num_get()
{
    return list_len(&timeslice_task_list);
}

void qtask_init(QTaskObj* task, 
                         const char* name,
                         void (*task_hdl)(void),
                         unsigned int timeslice, 
                         const char* usage)
{
    timeslice_task_id ++;
    task->name = name;
    task->id = timeslice_task_id;
    task->is_run = TASK_STOP;
    task->task_hdl = task_hdl;
    task->timer = timeslice;
    task->timeslice = timeslice;
    task->usage = usage;
    task->run_time = 0;
}

int qtask_add(QTaskObj* task)
{
    if(qdtask_isexsit(task) == 1) {
        task->is_run = TASK_STOP;
        task->timer = task->timeslice;
        list_remove(&task->qtask_node);
    }

    if(qtask_isexist(task) == 0) {
        list_insert_before(&timeslice_task_list, &task->qtask_node);
        return 0;
    }
    return -1;
}

int qtask_del(QTaskObj* task)
{
    if(qtask_isexist(task) == 1){
        task->is_run = TASK_STOP;
        task->timer = task->timeslice;
        list_remove(&task->qtask_node);
    }

    if(qdtask_isexsit(task) == 0){
        list_insert_before(&timeslice_task_del_list, &task->qtask_node);
        return 0;
    } else {
        return -1;
    }
}


int qtask_isexist(QTaskObj* task)
{
    ListObj *node, *_node;
    QTaskObj *_task;

    list_for_each_safe(node, _node, &timeslice_task_list) {
        _task = list_entry(node, QTaskObj, qtask_node);
        if(task->id == _task->id) {
            return 1;
        } else {
            continue;
        }
    }
    return 0;
}

int qdtask_isexsit(QTaskObj* task)
{
    ListObj *node, *_node;
    QTaskObj *_task;

    list_for_each_safe(node, _node, &timeslice_task_del_list) {
        _task = list_entry(node, QTaskObj, qtask_node);
        if(task->id == _task->id) {
            return 1;
        } else {
            continue;
        }
    }
    return 0;
}

unsigned int qtask_timeslice_get(QTaskObj* task)
{
    return task->timeslice;
}

void qtask_timeslice_set(QTaskObj* obj, unsigned int slice_len)
{
    obj->timeslice = slice_len;
}

QTaskObj* qtask_get(unsigned int task_id)
{
    ListObj* node = &timeslice_task_list;

    for(unsigned int i = task_id; i > 0; i--) {
        node = node->next;
    }

    return list_entry(node, QTaskObj, qtask_node);
}

unsigned int qdtask_num_get()
{
    return list_len(&timeslice_task_del_list);
}

QTaskObj* qdtask_get(unsigned int task_id)
{
    ListObj* node = &timeslice_task_del_list;

    for(unsigned int i = task_id; i > 0; i--) {
        node = node->next;
    }

    return list_entry(node, QTaskObj, qtask_node);
}
