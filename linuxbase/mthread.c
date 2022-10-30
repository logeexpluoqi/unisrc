/*
 * @Author: luoqi 
 * @Date: 2022-09-02 15:37:05 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-09-03 23:55:56
 */

#include <stdlib.h>
#include <sched.h>
#include <limits.h>
#include <unistd.h>
#include "mthread.h"
#include "../qshell/qsh.h"

static LIST_HEAD(mthread_list);
static LIST_HEAD(mthread_del_list);
static uint32_t thread_mid;

static int mthread_isexist(MThread *mthread);
static int mthread_del_isexist(MThread *mthread);

static int cmd_mls_hdl(int, char **);
static int cmd_mk_hdl(int, char **);
static int cmd_mr_hdl(int, char **);

int mthread_basic_init(void)
{
    thread_mid = 0;
    qcmd_export("mls", cmd_mls_hdl, "list all mthread info @ /-d");
    qcmd_export("mk", cmd_mk_hdl, "kill mthread @ mid");
    qcmd_export("mr", cmd_mr_hdl, "restart mthread @ mid");
    return 0;
}

int mthread_period_set(MThread *mthread, uint32_t per)
{
    mthread->period_us = per;
    return 0;
}

int mthread_periority_set(MThread *mthread, uint32_t pri)
{
    mthread->priority = pri;
    return 0;
}

int mthread_task_begin(MThread *mthread)
{
    int ret = clock_gettime(CLOCK_MONOTONIC, &mthread->time);
    if(mthread->time.tv_sec == mthread->time_k1.tv_sec){
        mthread->calltime = mthread->time.tv_nsec - mthread->time_k1.tv_nsec;
    }else{
        mthread->calltime = (uint64_t)(mthread->time.tv_sec - mthread->time_k1.tv_sec) * 1000000000 + mthread->time.tv_nsec - mthread->time_k1.tv_nsec;
    }
    mthread->time_k1 = mthread->time;
    return ret;
}

int mthread_task_end(MThread *mthread)
{
    struct timespec tv;
    int ret = clock_gettime(CLOCK_MONOTONIC, &tv);
    if(tv.tv_sec == mthread->time_k1.tv_sec){
        mthread->runtime = tv.tv_nsec - mthread->time_k1.tv_nsec;
    }else{
        mthread->runtime = (uint64_t)(tv.tv_sec - mthread->time_k1.tv_sec) * 1000000000 + tv.tv_nsec - mthread->time_k1.tv_nsec;
    }
    usleep(mthread->period_us);
    mthread->time_k1 = mthread->time;
    return ret;
}

int mthread_init(MThread *mthread, const char *name, uint8_t priority, uint32_t period_us, void *(*func)(void *), const char *usage)
{
    mthread->name           = name;
    mthread->priority       = priority;
    mthread->period_us      = period_us;
    mthread->func           = func;
    mthread->runtime        = 0;
    mthread->calltime       = 0;
    mthread->time.tv_sec    = 0;
    mthread->time.tv_nsec   = 0;
    mthread->usage          = usage;
    mthread->mid            = thread_mid++;
    return 0;
}

int mthread_start(MThread *mthread)
{
    int ret = -1;
    struct sched_param param;
    pthread_attr_t attr;
    param.sched_priority = mthread->priority;
    if(mthread_del_isexist(mthread)){
        list_remove(&mthread->mnode);
    }
#ifndef LINUX_USING_RTKERNEL
    if(mthread_isexist(mthread) == 0){
        list_insert_after(&mthread_list, &mthread->mnode);
        if(pthread_attr_init(&attr) != 0){
            printf(" #! thread %s, id %d attr init error !\r\n", mthread->name, mthread->mid);
        }
        if(pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN) != 0){
            printf(" #! thread %s, id %d attr setstacksize error !\r\n", mthread->name, mthread->mid);
        }
        if(pthread_attr_setschedpolicy(&attr, SCHED_FIFO) != 0){
            printf(" #! thread %s, id %d attr setschedpolicy error !\r\n", mthread->name, mthread->mid);
        }
        if(pthread_attr_getschedparam(&attr, &param) != 0){
            printf(" #! thread %s, id %d attr param error !\r\n", mthread->name, mthread->mid);
        }
        if(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0){
            printf(" #! thread %s, id %d attr setinheritsched error !\r\n", mthread->name, mthread->mid);
        }
        ret = pthread_create(&mthread->thread, &attr, mthread->func, NULL);
        pthread_detach(mthread->thread);
    }
#else
    list_insert_after(&mthread_list, &mthread->mnode);
    ret = pthread_create(&mthread->thread, NULL, mthread->func, NULL);
    pthread_detach(mthread->thread);
#endif
    return ret;
}

int mthread_stop(MThread *mthread)
{
    list_remove(&mthread->mnode);
    if(mthread_del_isexist(mthread) == 0){
        list_insert_after(&mthread_del_list, &mthread->mnode);
        mthread->time.tv_sec = 0;
        mthread->time.tv_nsec = 0;
        mthread->runtime = 0;
    }
    return pthread_cancel(mthread->thread);
}

int mthread_detach(MThread *mthread)
{
    return pthread_detach(mthread->thread);
}

int mthread_join(MThread *mthread)
{
    return pthread_join(mthread->thread, NULL);
}

int mthread_isexist(MThread *mthread)
{
    ListObj *node, *_node;
    list_for_each_safe(node, _node, &mthread_list){
        MThread *_mthread = list_entry(node, MThread, mnode);
        if(mthread->mid == _mthread->mid){
            return 1;
        }else{
            continue;
        }
    }
    return 0;
}

int mthread_del_isexist(MThread *mthread)
{
    ListObj *node, *_node;
    list_for_each_safe(node, _node, &mthread_del_list){
        MThread *_mthread = list_entry(node, MThread, mnode);
        if(mthread->mid == _mthread->mid){
            return 1;
        }else{
            continue;
        }
    }
    return 0;
}

int cmd_mls_hdl(int argc, char **argv)
{
    if(argc == 1){
        ListObj *node, *_node;
        uint32_t len = list_len(&mthread_list);
        printf(" Threads <%u>\r\n", len);
        printf(" [name]            [mid]    [period/us]    [runtime/us]     [calltime/us]     [priority]    [usage]\r\n");
        printf("--------          -------  -------------  --------------   ---------------   ------------  ---------\r\n");
        list_for_each_safe(node, _node, &mthread_list){
            MThread *_mthread = list_entry(node, MThread, mnode);
            if(_mthread != NULL){
                printf(" %-15s    %-5u    %-9u      %010.3f       %011.3f       %-3u           %-s\r\n",
                _mthread->name, _mthread->mid, _mthread->period_us, (float)(_mthread->runtime) / 1000,
                (float)(_mthread->calltime)  / 1000, _mthread->priority, _mthread->usage);
            }else{
                return CMD_NO_CMD;
            }
        }
        printf("\r\n");
    }else{
        if(ISARG(argv[1], "-d")){
            ListObj *node, *_node;
            uint32_t len = list_len(&mthread_list);
            printf(" Threads <%u>\r\n", len);
            printf(" [name]            [mid]    [period/us]    [runtime/us]     [calltime/us]     [priority]    [usage]\r\n");
            printf("--------          -------  -------------  --------------   ---------------   ------------  ---------\r\n");
            list_for_each_safe(node, _node, &mthread_del_list){
                MThread *_mthread = list_entry(node, MThread, mnode);
                if(_mthread != NULL){
                    printf(" %-15s    %-5u    %-9u      %010.3f       %011.3f       %-3u           %-s\r\n",
                    _mthread->name, _mthread->mid, _mthread->period_us, (float)(_mthread->runtime) / 1000,
                    (float)(_mthread->calltime)  / 1000, _mthread->priority, _mthread->usage);
                }else{
                    return CMD_NO_CMD;
                }
            }
            printf("\r\n");
        }else{
            return CMD_PARAM_ERR;
        }
    }
    return 0;
}

int cmd_mk_hdl(int argc, char **argv)
{
    if(argc <= 1){
        return CMD_PARAM_LESS;
    }else if(argc > 2){
        return CMD_PARAM_EXCEED;
    }else{
        ListObj *node, *_node;
        list_for_each_safe(node, _node, &mthread_list){
            MThread *_mthread = list_entry(node, MThread, mnode);
            if(_mthread->mid == atol(argv[1])){
                mthread_stop(_mthread);
                return 0;
            }
        }
    }
    return 0;
}

int cmd_mr_hdl(int argc, char **argv)
{
    if(argc <= 1){
        return CMD_PARAM_LESS;
    }else if(argc > 2){
        return CMD_PARAM_EXCEED;
    }else{
        ListObj *node, *_node;
        list_for_each_safe(node, _node, &mthread_del_list){
            MThread *_mthread = list_entry(node, MThread, mnode);
            if(_mthread->mid == atol(argv[1])){
                mthread_start(_mthread);
                return 0;
            }
        }
    }
    return 0;
}
