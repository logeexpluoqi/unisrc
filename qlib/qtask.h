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

#include <stdint.h>
#include "qlist.h"

typedef enum {
    TASK_STOP,
    TASK_RUN
} IsTaskRun;

typedef void (*QTaskHandle)(void);
typedef struct _qtask
{
    const char* name;
    uint32_t id;
    QTaskHandle handle;
    IsTaskRun is_run;
    uint32_t timer;
    uint32_t tick;
    uint32_t run_time;
    QList qtask_node;
    const char* usage;
} QTaskObj;

void qtask_exec(void);

void qtask_tick(void);

void qtask_init(QTaskObj* task, 
                         const char* name, 
                         QTaskHandle handle,
                         uint32_t tick, 
                         const char* usage);

int qtask_add(QTaskObj* task);

int qtask_del(QTaskObj* task);

void qtask_tick_set(QTaskObj* task, uint32_t tick);

uint32_t qtask_num(void);

uint32_t qdtask_num(void);

int qtask_isexist(QTaskObj* task);

int qdtask_isexsit(QTaskObj* task);

QTaskObj* qtask_get(uint32_t task_id);

QTaskObj* qdtask_get(uint32_t task_id);

#ifdef __cplusplus
 }
#endif

#endif
