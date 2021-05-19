/*
 * @Author: luoqi 
 * @Date: 2021-05-13 09:59:48 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-05-19 18:11:51
 */

#include "fsm.h"

void fsm_init(FsmObj *obj, const char* name, int curr_state, int next_state)
{
    obj->name = name;
    obj->curr_state = curr_state;
    obj->next_state = next_state;
    obj->fsm_state_id_base = 0;
    list_init(&obj->fsm_task_list);    
}

unsigned char fsm_exec(FsmObj *obj)
{
    ListObj* node;
    FsmStateObj* state;
    
    obj->curr_state = obj->next_state;
    list_for_each(node, &obj->fsm_task_list)
    {
        state = list_entry(node, FsmStateObj, fsm_state_list);
        if(obj->curr_state == state->link_state)
        {
           return state->fsm_state_task_hdl();
        }
        else 
            return 0;
    }
    return 0;
}

void fsm_change_state(FsmObj* obj, int next_state)
{
    obj->next_state = next_state;
}

void fsm_state_init(FsmStateObj* obj, int link_state, unsigned char (*fsm_state_task_hdl)(void))
{
    obj->id = 0;
    obj->link_state = link_state;
    obj->fsm_state_task_hdl = fsm_state_task_hdl;
}

void fsm_state_add(FsmObj* fsm_obj, FsmStateObj* state_obj)
{
    if(state_obj->id == 0)
    {
        fsm_obj->fsm_state_id_base ++;
        state_obj->id = fsm_obj->fsm_state_id_base;
    }
    state_obj->belong_to = fsm_obj->name;
    list_insert_before(&fsm_obj->fsm_task_list, &state_obj->fsm_state_list);
}

void fsm_state_del(FsmStateObj* obj)
{
    list_remove(&obj->fsm_state_list);
}

int fsm_state_get(FsmObj* obj)
{
    return obj->curr_state;
}

const char* fsm_state_blelong_to(FsmStateObj* obj)
{
    return obj->belong_to;    
}


