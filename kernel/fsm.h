/*
 * @Author: luoqi 
 * @Date: 2021-05-13 09:57:45 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-05-19 18:03:22
 */

#ifndef _FSM_H
#define _FSM_H

#include "service/list.h"

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
    unsigned char (*fsm_state_task_hdl)(void);
    ListObj fsm_state_list;
} FsmStateObj;

void fsm_init(FsmObj* obj, const char* name, int init_state);
void fsm_change_state(FsmObj* obj, int next_state);
int fsm_state_get(FsmObj* obj);
unsigned char fsm_exec(FsmObj* obj);

void fsm_state_init(FsmStateObj* obj, int link_state, unsigned char (*fsm_state_task_hdl)(void));
void fsm_state_add(FsmObj* fsm_obj, FsmStateObj* state_obj);
void fsm_state_del(FsmStateObj* obj);
int fsm_state_link(FsmStateObj* obj);
const char* fsm_state_belong_to(FsmStateObj* obj);

#endif
