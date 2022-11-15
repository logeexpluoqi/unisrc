/*
 * @Author: luoqi 
 * @Date: 2021-08-31 15:49:21 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 19:25:13
 */

#include <string.h>
#include "demo_fsm.h"
#include "../frame/fsm.h"
#include "../qshell/qsh.h"

static FsmObj fsm_1;
static FsmStateObj state_1, state_2, state_3, state_4;
static int state_1_hdl(void);
static int state_2_hdl(void);
static int state_3_hdl(void);
static int state_4_hdl(void);

static FsmObj fsm_2;
static FsmStateObj state_5, state_6, state_7, state_8;
static int state_5_hdl(void);
static int state_6_hdl(void);
static int state_7_hdl(void);
static int state_8_hdl(void);

FsmStateObj state_idle;
static int state_idle_hdl(void);

static CmdObj cmd_fsm;
static int cmd_fsm_hdl(int argc, char **argv);

int demo_fsm_init()
{
    qcmd_init(&cmd_fsm, "fsm", cmd_fsm_hdl, "@ fsm<run/stop>");
    qcmd_add(&cmd_fsm);
    fsm_init(&fsm_1, "fsm 1", 0);
    fsm_init(&fsm_2, "fsm 2", 0);


    return 0;
}

int cmd_fsm_hdl(int argc, char **argv)
{
    if(argc <= 1) {
        return CMD_PARAM_LESS;
    }

    if(strcmp(argv[1], "run") == 0) {

        QSH(">> fsm funning\r\n");
    } else if(strcmp(argv[1], "stop") == 0) {

        QSH(">> fsm stopped\r\n");
    } else {
        return CMD_PARAM_ERR;
    }
    return 0;
}
