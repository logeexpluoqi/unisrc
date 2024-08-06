/*
 * @Author: luoqi
 * @Date: 2021-04-29 19:27:09
 * @ Modified by: luoqi
 * @ Modified time: 2024-08-06 21:24
 */

#include "qtask.h"

#define QTASK_LIST_ITERATOR(node, list)         for (node = (list)->next; node != (list); node = node->next)
#define QTASK_LIST_ENTRY(ptr, type, member)     ((type *)((char *)(ptr) - ((unsigned long) &((type*)0)->member)))

static inline void _list_insert(QTaskList *list, QTaskList *node)
{
    list->next->prev = node;
    node->next = list->next;

    list->next = node;
    node->prev = list;
}

static inline void _list_remove(QTaskList *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;

    node->next = node->prev = node;
}

static int qtask_isexist(QTaskScheduler *sched, QTaskObj *task)
{
    QTaskList *node;
    QTaskObj *_task;

    QTASK_LIST_ITERATOR(node, &sched->sched_list) {
        _task = QTASK_LIST_ENTRY(node, QTaskObj, task_node);
        if(task->id == _task->id) {
            return 1;
        } else {
            continue;
        }
    }
    return 0;
}

static int qdtask_isexsit(QTaskScheduler *sched, QTaskObj *task)
{
    QTaskList *node;
    QTaskObj *_task;

    QTASK_LIST_ITERATOR(node, &sched->unsched_list) {
        _task = QTASK_LIST_ENTRY(node, QTaskObj, task_node);
        if(task->id == _task->id) {
            return 1;
        } else {
            continue;
        }
    }
    return 0;
}


void qtask_scheduler_init(QTaskScheduler *sched)
{
    sched->id = 0;
    sched->sched_list.prev = sched->sched_list.next = &sched->sched_list;
    sched->unsched_list.prev = sched->unsched_list.next = &sched->unsched_list;
}

int qtask_attach(QTaskScheduler *sched, QTaskObj *task, const char *name, QTaskHandle handle, uint32_t tick, const char *usage)
{
    sched->id++;
    task->name = name;
    task->id = sched->id;
    task->isready = 0;
    task->handle = handle;
    task->timer = tick;
    task->tick = tick;
    task->usage = usage;
    task->rtime = 0;
    if(qdtask_isexsit(sched, task)) {
        task->isready = 0;
        task->timer = task->tick;
        _list_remove(&task->task_node);
    }

    if(!qtask_isexist(sched, task)) {
        _list_insert(&sched->sched_list, &task->task_node);
        return 0;
    }
    return 1;
}

int qtask_detach(QTaskScheduler *sched, QTaskObj *task)
{
    if(qtask_isexist(sched, task)) {
        task->isready = 0;
        task->timer = task->tick;
        _list_remove(&task->task_node);
    }

    if(!qdtask_isexsit(sched, task)) {
        _list_insert(&sched->unsched_list, &task->task_node);
        return 0;
    } else {
        return -1;
    }
}

void qtask_exec(QTaskScheduler *sched)
{
    QTaskList *node;
    QTaskObj *task;

    QTASK_LIST_ITERATOR(node, &sched->sched_list) {
        task = QTASK_LIST_ENTRY(node, QTaskObj, task_node);
        if(task->isready) {
            task->handle();
            task->isready = 0;
        }
    }
}

void qtask_tick_increase(QTaskScheduler *sched)
{
    QTaskList *node;
    QTaskObj *task;

    QTASK_LIST_ITERATOR(node, &sched->sched_list) {
        task = QTASK_LIST_ENTRY(node, QTaskObj, task_node);
        if(task->timer != 0) {
            task->timer--;
            if(task->timer == 0) {
                task->isready = 1;
                task->timer = task->tick;
            }
        }
    }
}

void qtask_tick_set(QTaskObj *obj, uint32_t tick)
{
    obj->tick = tick;
}
