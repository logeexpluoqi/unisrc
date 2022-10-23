/*
 * @Author: luoqi 
 * @Date: 2022-08-13 22:37:49 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-08-14 00:14:59
 */
#include <stdio.h>
#include <math.h>
#include "demo_solver.h"
#include "../solver/ode_euler.h"
#include "../qshell/qsh.h"

static CmdObj cmd_ode;
static int cmd_ode_hdl(int argc, char **argv);

#define TSPAN_SIZE      1000
static float tspan[TSPAN_SIZE];

int demo_solver_init()
{
    qsh_cmd_init(&cmd_ode, "ode", cmd_ode_hdl, "@ euler");
    qsh_cmd_add(&cmd_ode);
}

static float dy(float t)
{
    return cosf(t);
}

static int demo_euler_solver()
{
    float yt[TSPAN_SIZE] = {0};
    ode_euler(yt, dy, 0.01, 0, 0, TSPAN_SIZE);
    for(int i = 0; i < TSPAN_SIZE; i++){
        if(i % 10 == 0){
            printf("\r\n");
        }
        printf(" %f", yt[i]);
    }
    printf("\r\n");
}

int cmd_ode_hdl(int argc, char **argv)
{
    if(argc <= 1){
        return CMD_PARAM_LESS;
    }

    if(ISARG(argv[1], "euler")){
        demo_euler_solver();
    }else{
        return CMD_PARAM_ERR;
    }

    return 0;
}
