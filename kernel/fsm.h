/*
 * @Author: luoqi 
 * @Date: 2021-05-13 09:57:45 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 19:23:59
 */

#ifndef _FSM_H
#define _FSM_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "qlist.h"

typedef struct fsm_object {
    const char* name;
    unsigned int fsm_state_id_base;
    int curr_state;
    int next_state;
    ListObj fsm_list_head;
} FsmObj;

typedef struct fsm_state_object {
    unsigned int id;
    const char* belong_to;
    int link_state;
    int (*fsm_state_task_hdl)(void);
    ListObj fsm_state_list;
} FsmStateObj;

void fsm_init(FsmObj* fsm, const char* name, int init_state);

void fsm_change_state(FsmObj* fsm, int next_state);

int fsm_state_get(FsmObj* fsm);

int fsm_exec(FsmObj* fsm);

void fsm_state_init(FsmStateObj* state, int link_state, int (*fsm_state_task_hdl)(void));

void fsm_state_add(FsmObj* fsm, FsmStateObj* state);

void fsm_state_del(FsmStateObj* state);

int fsm_state_link(FsmStateObj* state);

const char* fsm_state_belong_to(FsmStateObj* state);

#ifdef __cplusplus
 }
#endif

#endif
