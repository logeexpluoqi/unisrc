/*
 * @Author: luoqi 
 * @Date: 2022-08-13 22:37:49 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-08-14 00:14:59
 */

#include "demo_solver.h"
#include "../solver/ode_euler.h"
#include "../console/qsh.h"

static CmdObj cmd_ode;
static int cmd_ode_hdl(int argc, char **argv);

#define TSPAN_SIZE      10000
static float tspan[TSPAN_SIZE];

int demo_solver_init()
{
    qsh_cmd_init(&cmd_ode, "ode", cmd_ode_hdl, "@ euler");   
}


int cmd_ode_hdl(int argc, char **argv)
{
    if(argc <= 1){
        return CMD_PARAM_LESS;
    }

    if(QSH_ISARG(argv[1], "euler")){

    }else{
        return CMD_PARAM_ERR;
    }

    return 0;
}
