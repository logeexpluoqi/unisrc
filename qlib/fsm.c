/*
 * @Author: luoqi
 * @Date: 2021-05-13 09:59:48
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 19:24:22
 */

#include "fsm.h"

void fsm_init(FsmObj *fsm, const char *name, int init_state)
{
    fsm->name = name;
    fsm->curr_state = init_state;
    fsm->next_state = init_state;
    fsm->id_base = 0;
    qlist_init(&fsm->state_list);
}

int fsm_exec(FsmObj *fsm)
{
    QList *node;
    FsmStateObj *state;

    fsm->curr_state = fsm->next_state;
    QLIST_ITERATOR(node, &fsm->state_list)
    {
        state = QLIST_ENTRY(node, FsmStateObj, state_node);
        if(state->link_state == fsm->curr_state) {
            return state->callback();
        } else {
            continue;
        }
    }
    return -1;
}

void fsm_state_update(FsmObj *fsm, int next_state)
{
    fsm->next_state = next_state;
}

void fsm_state_attach(FsmObj *fsm, FsmStateObj *state, int link_state, int (*callback)(void))
{
    fsm->id_base++;
    state->id = fsm->id_base;
    state->link_state = link_state;
    state->callback = callback;
    qlist_insert_after(&fsm->state_list, &state->state_node);
}

void fsm_state_detach(FsmStateObj *state)
{
    qlist_remove(&state->state_node);
}
