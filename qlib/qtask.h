/*
 * @Author: luoqi 
 * @Date: 2021-04-29 19:27:49 
 * @ Modified by: luoqi
 * @ Modified time: 2024-10-12 11:13
 */

#ifndef _QTASK_H
#define _QTASK_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define QTASK_NONE ((void*)0)

typedef struct _task_list {
    struct _task_list* prev;
    struct _task_list* next;
} QTaskList;

typedef struct _qtask
{
    const char* name;
    uint32_t id : 30;
    uint32_t isready : 1;
    void (*handle)(void *args);
    uint32_t timer;
    uint32_t tick;
    uint32_t rtime;
    uint32_t rtick;
    QTaskList task_node;
    const char* usage;
} QTaskObj;
typedef void (*QTaskHandle)(void *args);

typedef struct
{
    void *args;
    uint32_t targetid;
    QTaskObj *run_task;
    QTaskList sched_list;
    QTaskList unsched_list;
} QTaskScheduler;


void qtask_scheduler_init(QTaskScheduler *sched);

int qtask_attach(QTaskScheduler *sched, QTaskObj* task, const char* name, QTaskHandle handle, uint32_t tick, const char* usage);

int qtask_detach(QTaskScheduler *sched, QTaskObj* task);

void qtask_exec(QTaskScheduler *sched);

void qtask_param_set(QTaskScheduler *sched, const char *taskname, void *args);

QTaskObj *qtask_get(QTaskScheduler *sched, const char *taskname);

void qtask_tick_increase(QTaskScheduler *sched);

/** 
 * It is used to measure the task callback function execution time.
 * It must be called in a timer interrupt function faster than qtask_tick_increase.
 */
void qtask_runtime_increase(QTaskScheduler *sched);

/** 
 * Change task periodic time onece 
 */
void qtask_sleep(QTaskScheduler *sched, uint32_t tick);

/** 
 * Change task periodic time 
 */
void qtask_tick_set(QTaskObj* task, uint32_t tick);

#ifdef __cplusplus
 }
#endif

#endif
