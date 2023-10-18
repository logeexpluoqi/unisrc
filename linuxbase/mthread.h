/*
 * @Author: luoqi 
 * @Date: 2022-09-02 15:20:22 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-09-03 23:51:21
 */

#ifndef _MTHREAD_H
#define _MTHREAD_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <pthread.h>
#include <stdint.h>
#include <time.h>
#include "../qlib/qlist.h"

#define LINUX_USING_RTKERNEL
typedef struct mthread
{
    const char          *name;
    pthread_t           thread;
    uint32_t            period_us;
    uint32_t            runtime;
    uint32_t            calltime;
    struct timespec     time;
    struct timespec     time_k1;
    uint32_t            mid;
    uint8_t             priority;
    void                *(*func)(void *);
    const char          *usage;
    QList             mnode;
} MThread;

int mthread_period_set(MThread *mthread, uint32_t per);

int mthread_periority_set(MThread *mthread, uint32_t per);

int mthread_task_begin(MThread *mthread);

int mthread_task_end(MThread *mthread);

int mthread_init(MThread *mthread, const char *name, uint8_t priority, uint32_t period_us, void *(*func)(void *), const char *usage);

int mthread_start(MThread *mthread);

int mthread_stop(MThread *mthread);

int mthread_detach(MThread *mthread);

int mthread_join(MThread *mthread);

#ifdef __cplusplus
 }
#endif

#endif
