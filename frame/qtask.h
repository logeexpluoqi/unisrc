/*
 * @Author: luoqi 
 * @Date: 2021-04-29 19:27:49 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 16:19:43
 */

#ifndef _QTASK_H
#define _QTASK_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "qlist.h"

typedef enum {
    TASK_STOP,
    TASK_RUN
} IsTaskRun;

typedef struct _qtask
{
    const char* name;
    unsigned int id;
    void (*task_hdl)(void);
    IsTaskRun is_run;
    unsigned int timer;
    unsigned int timeslice;
    unsigned long long run_time;
    ListObj qtask_node;
    const char* usage;
} QTaskObj;

void qtask_exec(void);

void qtask_tick(void);

void qtask_init(QTaskObj* task, 
                         const char* name, 
                         void (*task_hdl)(void),
                         unsigned int timeslice, 
                         const char* usage);

int qtask_add(QTaskObj* task);

int qtask_del(QTaskObj* task);

void qtask_timeslice_set(QTaskObj* task, unsigned int slice_len);

unsigned int qtask_timeslice_get(QTaskObj* task);

unsigned int qtask_num_get(void);

int qtask_isexist(QTaskObj* task);

int qdtask_isexsit(QTaskObj* task);

QTaskObj* qtask_get(unsigned int task_id);

unsigned int qdtask_num_get(void);

QTaskObj* qdtask_get(unsigned int task_id);

#ifdef __cplusplus
 }
#endif

#endif
