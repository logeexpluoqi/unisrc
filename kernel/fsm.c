/*
 * @Author: luoqi 
 * @Date: 2021-05-13 09:59:48 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 19:24:22
 */

#include "fsm.h"

void fsm_init(FsmObj* fsm, const char* name, int init_state)
{
    fsm->name = name;
    fsm->curr_state = init_state;
    fsm->next_state = init_state;
    fsm->fsm_state_id_base = 0;
    list_init(&fsm->fsm_list_head);
}

int fsm_exec(FsmObj* fsm)
{
    ListObj* node;
    FsmStateObj* state;

    fsm->curr_state = fsm->next_state;
    list_for_each(node, &fsm->fsm_list_head) {
        state = list_entry(node, FsmStateObj, fsm_state_list);
        if (state->link_state == fsm->curr_state) {
            return state->fsm_state_task_hdl();
        }
    }
    return -1;
}

void fsm_change_state(FsmObj* fsm, int next_state)
{
    fsm->next_state = next_state;
}

void fsm_state_init(FsmStateObj* state, int link_state, int (*fsm_state_task_hdl)(void))
{
    state->id = 0;
    state->link_state = link_state;
    state->fsm_state_task_hdl = fsm_state_task_hdl;
}

void fsm_state_add(FsmObj* fsm, FsmStateObj* state)
{
    if (state->id == 0) {
        fsm->fsm_state_id_base++;
        state->id = fsm->fsm_state_id_base;
    }
    state->belong_to = fsm->name;
    list_insert_before(&fsm->fsm_list_head, &state->fsm_state_list);
}

void fsm_state_del(FsmStateObj* state)
{
    list_remove(&state->fsm_state_list);
}

int fsm_state_get(FsmObj* state)
{
    return state->curr_state;
}

int fsm_state_link(FsmStateObj* state)
{
    return state->link_state;
}

const char* fsm_state_belong_to(FsmStateObj* state)
{
    return state->belong_to;
}


