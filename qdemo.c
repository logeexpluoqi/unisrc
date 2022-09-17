/*
 * @Author: luoqi 
 * @Date: 2021-08-31 15:09:54 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-09-05 11:29:56
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "qshell/qsh.h"
#include "frame/timeslice.h"
#include "demo/demo_qsh.h"
#include "demo/demo_fsm.h"
#include "demo/demo_timeslice.h"
#include "demo/demo_msg.h"
#include "demo/demo_qmath.h"
#include "demo/demo_qkey.h"
#include "demo/demo_filter.h"
#include "demo/demo_sort.h"
#include "demo/demo_solver.h"
#include "demo/demo_mthread.h"
#include "demo/demo_udp_server.h"

static pthread_t tid_qsh_isr;
static void* thread_qsh_input_isr(void*);

static pthread_t tid_tasks;
static void* thread_tasks(void*);

static int close_all = 0;

int main()
{
    pthread_create(&tid_qsh_isr, NULL, thread_qsh_input_isr, NULL);
    pthread_create(&tid_tasks, NULL, thread_tasks, NULL);

    qsh_init();

    demo_fsm_init();
    demo_timeslice_init();
    demo_msg_init();
    demo_qsh_init();
    demo_qmath_init();
    demo_qkey_init();
    demo_filter_init();
    demo_sort_init();
    demo_solver_init();
    demo_mthread_init();
    demo_udp_server_init();

    for(;;) {
        if(close_all == 0) {
            qsh_exec();
            timeslice_exec();
            usleep(10);
        } else {
            return 0;
        }
    }
    return 0;
}

void* thread_qsh_input_isr(void* param)
{
    char ch;
    for(;;) {
        system("stty raw -echo");
        ch = getchar();
        ch = (ch == 127) ? 8 : ch;
        if(ch != 3) {
            qsh_recv(ch);
        } else {
            system("stty -raw echo");
            printf("\33[2K");
            printf("\033[H\033[J");
            printf(" \r\n#! qsh input thread closed !\r\n\r\n");
            close_all = 1;
            pthread_cancel(tid_tasks);
            pthread_cancel(tid_qsh_isr);
        }
        usleep(1e4);
    }
}

void* thread_tasks(void* param)
{
    for(;;) {
        timeslice_tick();
        usleep(1e3);
    }
}

