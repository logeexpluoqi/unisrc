/*
 * @Author: luoqi
 * @Date: 2021-04-29 19:27:09
 * @ Modified by: luoqi
 * @ Modified time: 2024-10-12 11:13
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

static int _qtask_isexist(QTaskScheduler *sched, QTaskObj *task)
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

static int _qdtask_isexsit(QTaskScheduler *sched, QTaskObj *task)
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

static uint16_t _id_calc(const char *name)
{
    uint16_t id = 0;
    for(int i = 0; name[i] != '\0'; i++) {
        id += (name[i] + i);
    }
    return id;
}

void qtask_scheduler_init(QTaskScheduler *sched)
{
    sched->sched_list.prev = sched->sched_list.next = &sched->sched_list;
    sched->unsched_list.prev = sched->unsched_list.next = &sched->unsched_list;
}

int qtask_attach(QTaskScheduler *sched, QTaskObj *task, const char *name, QTaskHandle handle, uint32_t tick, const char *usage)
{
    task->name = name;
    task->id = _id_calc(name);
    task->isready = 0;
    task->handle = handle;
    task->timer = tick;
    task->tick = tick;
    task->usage = usage;
    task->rtime = 0;
    task->rtick = 0;
    if(_qdtask_isexsit(sched, task)) {
        task->isready = 0;
        task->timer = task->tick;
        _list_remove(&task->task_node);
    }

    if(!_qtask_isexist(sched, task)) {
        _list_insert(&sched->sched_list, &task->task_node);
        return 0;
    }
    return 1;
}

int qtask_detach(QTaskScheduler *sched, QTaskObj *task)
{
    if(_qtask_isexist(sched, task)) {
        task->isready = 0;
        task->timer = task->tick;
        _list_remove(&task->task_node);
    }

    if(!_qdtask_isexsit(sched, task)) {
        _list_insert(&sched->unsched_list, &task->task_node);
        return 0;
    } else {
        return -1;
    }
}

QTaskObj *qtask_get(QTaskScheduler *sched, const char *taskname)
{
    QTaskList *node;
    QTaskObj *task;
    uint16_t id = _id_calc(taskname);
    QTASK_LIST_ITERATOR(node, &sched->sched_list) {
        task = QTASK_LIST_ENTRY(node, QTaskObj, task_node);
        if(task->id == id) {
            return task;
        } else {
            continue;
        }
    }
    return QTASK_NONE;
}

void qtask_exec(QTaskScheduler *sched)
{
    QTaskList *node;
    QTaskObj *task;
    void *args;
    QTASK_LIST_ITERATOR(node, &sched->sched_list) {
        task = QTASK_LIST_ENTRY(node, QTaskObj, task_node);
        sched->run_task = task;
        if(sched->targetid == task->id) {
            args = sched->args;
        } else {
            args = QTASK_NONE;
        }
        if(task->isready) {
            task->handle(args);
            task->isready = 0;
            task->rtime = task->rtick;
            task->rtick = 0;
        }
    }
}

void qtask_param_set(QTaskScheduler *sched, const char *taskname, void *args)
{
    sched->targetid = _id_calc(taskname);
    sched->args = args;
}

void qtask_tick_increase(QTaskScheduler *sched)
{
    QTaskList *node;
    QTaskObj *task;

    QTASK_LIST_ITERATOR(node, &sched->sched_list) {
        task = QTASK_LIST_ENTRY(node, QTaskObj, task_node);
        if(task->timer > 0) {
            task->timer--;
            if(task->timer == 0) {
                task->isready = 1;
                task->timer = task->tick;
            }
        }
    }
}

void qtask_runtime_increase(QTaskScheduler *sched)
{
    QTaskList *node;
    QTaskObj *task;

    QTASK_LIST_ITERATOR(node, &sched->sched_list) {
        task = QTASK_LIST_ENTRY(node, QTaskObj, task_node);
        if(task->isready) {
            task->rtick++;
        }
    }
}

void qtask_sleep(QTaskScheduler *sched, uint32_t tick)
{
    if(sched->run_task != QTASK_NONE) {
        sched->run_task->timer = tick;
    }
}

void qtask_tick_set(QTaskObj *obj, uint32_t tick)
{
    obj->tick = tick;
}
