/*
 * @Author: luoqi 
 * @Date: 2021-04-29 19:27:49 
 * @ Modified by: luoqi
 * @ Modified time: 2024-08-06 21:24
 */

#ifndef _QTASK_H
#define _QTASK_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

typedef struct _task_list {
    struct _task_list* prev;
    struct _task_list* next;
} QTaskList;

typedef struct
{
    uint32_t id;
    QTaskList sched_list;
    QTaskList unsched_list;
} QTaskScheduler;

typedef void (*QTaskHandle)(void);
typedef struct _qtask
{
    const char* name;
    uint32_t isready : 1;
    uint32_t id;
    QTaskHandle handle;
    uint32_t timer;
    uint32_t tick;
    uint32_t rtime;
    QTaskList task_node;
    const char* usage;
} QTaskObj;

void qtask_scheduler_init(QTaskScheduler *sched);

int qtask_attach(QTaskScheduler *sched, QTaskObj* task, const char* name, QTaskHandle handle, uint32_t tick, const char* usage);

int qtask_detach(QTaskScheduler *sched, QTaskObj* task);

void qtask_exec(QTaskScheduler *sched);

void qtask_tick_increase(QTaskScheduler *sched);

void qtask_tick_set(QTaskObj* task, uint32_t tick);

#ifdef __cplusplus
 }
#endif

#endif
