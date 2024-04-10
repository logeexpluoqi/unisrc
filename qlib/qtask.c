/*
 * @Author: luoqi 
 * @Date: 2021-04-29 19:27:09 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 18:43:45
 */

#include "qtask.h"

static QLIST_CREATE(qtask_list);
static QLIST_CREATE(qdtask_list);
static uint32_t qtask_id = 0;

void qtask_exec()
{
    QList *node, *_node;
    QTaskObj *task;

    QLIST_ITERATER_SAFE(node, _node, &qtask_list) {
        task = QLIST_ENTRY(node, QTaskObj, qtask_node);
        if(task->is_run == TASK_RUN) {
            task->handle();
            task->is_run = TASK_STOP;
        }
    }
}

void qtask_tick()
{
    QList* node;
    QTaskObj *task;

    QLIST_ITERATOR(node, &qtask_list) {
        task = QLIST_ENTRY(node, QTaskObj, qtask_node);
        if(task->timer != 0) {
            task->timer --;
            if(task->timer == 0) {
                task->is_run = TASK_RUN;
                task->timer = task->tick;
            }
        }
    }
}

uint32_t qtask_num()
{
    return qlist_len(&qtask_list);
}

void qtask_init(QTaskObj* task, 
                         const char* name,
                         QTaskHandle handle,
                         uint32_t tick, 
                         const char* usage)
{
    qtask_id ++;
    task->name = name;
    task->id = qtask_id;
    task->is_run = TASK_STOP;
    task->handle = handle;
    task->timer = tick;
    task->tick = tick;
    task->usage = usage;
    task->run_time = 0;
}

int qtask_add(QTaskObj* task)
{
    if(qdtask_isexsit(task) == 1) {
        task->is_run = TASK_STOP;
        task->timer = task->tick;
        qlist_remove(&task->qtask_node);
    }

    if(qtask_isexist(task) == 0) {
        qlist_insert_before(&qtask_list, &task->qtask_node);
        return 0;
    }
    return -1;
}

int qtask_del(QTaskObj* task)
{
    if(qtask_isexist(task) == 1){
        task->is_run = TASK_STOP;
        task->timer = task->tick;
        qlist_remove(&task->qtask_node);
    }

    if(qdtask_isexsit(task) == 0){
        qlist_insert_before(&qdtask_list, &task->qtask_node);
        return 0;
    } else {
        return -1;
    }
}


int qtask_isexist(QTaskObj* task)
{
    QList *node, *_node;
    QTaskObj *_task;

    QLIST_ITERATER_SAFE(node, _node, &qtask_list) {
        _task = QLIST_ENTRY(node, QTaskObj, qtask_node);
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
    QList *node, *_node;
    QTaskObj *_task;

    QLIST_ITERATER_SAFE(node, _node, &qdtask_list) {
        _task = QLIST_ENTRY(node, QTaskObj, qtask_node);
        if(task->id == _task->id) {
            return 1;
        } else {
            continue;
        }
    }
    return 0;
}

uint32_t qtask_tick_get(QTaskObj* task)
{
    return task->tick;
}

void qtask_tick_set(QTaskObj* obj, uint32_t tick)
{
    obj->tick = tick;
}

QTaskObj* qtask_get(uint32_t task_id)
{
    QList* node = &qtask_list;

    for(uint32_t i = task_id; i > 0; i--) {
        node = node->next;
    }

    return QLIST_ENTRY(node, QTaskObj, qtask_node);
}

uint32_t qdtask_num()
{
    return qlist_len(&qdtask_list);
}

QTaskObj* qdtask_get(uint32_t task_id)
{
    QList* node = &qdtask_list;

    for(uint32_t i = task_id; i > 0; i--) {
        node = node->next;
    }

    return QLIST_ENTRY(node, QTaskObj, qtask_node);
}
