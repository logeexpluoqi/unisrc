/*
 * @Author: luoqi 
 * @Date: 2022-11-22 22:55:05 
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-05-11 14:31:59
 */

#include "qdemo.h"

int qdemo_init()
{
    demo_fsm_init();
    demo_qtask_init();
    demo_msg_init();
    demo_qsh_init();
    demo_qmath_init();
    demo_qkey_init();
    demo_filter_init();
    demo_sort_init();
    demo_solver_init();
    demo_mthread_init();
    demo_udp_server_init();
    demo_qtrigf_init();
    demo_matrix_init();
    return 0;
}
