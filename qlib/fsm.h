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

#include <stdint.h>
#include "qlist.h"

typedef struct fsm_object {
    const char* name;
    uint32_t id_base;
    int curr_state;
    int next_state;
    QList state_list;
} FsmObj;

typedef struct fsm_state_object {
    uint32_t id;
    int link_state;
    int (*callback)(void);
    QList state_node;
} FsmStateObj;

void fsm_init(FsmObj* fsm, const char* name, int init_state);

void fsm_state_update(FsmObj* fsm, int next_state);

int fsm_exec(FsmObj* fsm);

void fsm_state_attach(FsmObj* fsm, FsmStateObj* state, int link_state, int (*callback)(void));

void fsm_state_detach(FsmStateObj* state);

#ifdef __cplusplus
 }
#endif

#endif
