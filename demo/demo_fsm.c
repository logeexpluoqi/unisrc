/*
 * @Author: luoqi 
 * @Date: 2021-08-31 15:49:21 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-08-31 17:33:03
 */

#include "demo_fsm.h"
#include "../kernel/fsm.h"
#include "../console/qsh.h"

static FsmObj fsm_1;
static FsmStateObj state_1, state_2, state_3, state_4;
static unsigned char state_1_hdl(void);
static unsigned char state_2_hdl(void);
static unsigned char state_3_hdl(void);
static unsigned char state_4_hdl(void);

static FsmObj fsm_2;
static FsmStateObj state_5, state_6, state_7, state_8;
static unsigned char state_5_hdl(void);
static unsigned char state_6_hdl(void);
static unsigned char state_7_hdl(void);
static unsigned char state_8_hdl(void);

FsmStateObj state_idle;
static unsigned char state_idle_hdl(void);

static QSH_CMD_CREAT(cmd_fsm);
static unsigned char cmd_fsm_hdl(int argc, char *argv[]);

int demo_fsm_init()
{
    qsh_cmd_init(&cmd_fsm, "fsm", 0xff, cmd_fsm_hdl, "<fsm run/stop>");
    qsh_cmd_add(&cmd_fsm);
    fsm_init(&fsm_1, "fsm 1", 0);
    fsm_init(&fsm_2, "fsm 2", 0);


    return 0;
}

unsigned char cmd_fsm_hdl(int argc, char *argvp[])
{

    return 0;
}
