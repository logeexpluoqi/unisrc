/*
 * @Author: luoqi 
 * @Date: 2022-08-13 22:37:49 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-27 21:44:32
 */
#include <stdio.h>
#include <math.h>
#include "qdemo.h"
#include "../solver/ode_euler.h"
#include "../solver/ode_rk.h"
#include "../qshell/qterm.h"

static QCmdObj cmd_ode;
static int cmd_ode_hdl(int argc, char **argv);

#define TSPAN_SIZE      31
static float tspan[TSPAN_SIZE];

int demo_solver_init()
{
    qterm_attatch(&cmd_ode, "ode", cmd_ode_hdl, "euler, rk, rkf45");
}

static float dy(float x, float y)
{
    return (sqrtf(x) / (y + 1) + cosf(x) + sinf(3 * y));
}

static int demo_euler_solver()
{
    float yt[TSPAN_SIZE] = {0};
    ode_euler_1st_calcu(yt, dy, 0.5, 0, dy(0, 0), TSPAN_SIZE);
    for(int i = 0; i < TSPAN_SIZE; i++){
        if(i % 10 == 0){
            printf("\r\n");
        }
        printf(" %f", yt[i]);
    }
    printf("\r\n");
    ode_euler_2st_calcu(yt, dy, 0.5, 0, dy(0, 0), TSPAN_SIZE);
    for(int i = 0; i < TSPAN_SIZE; i++){
        if(i % 10 == 0){
            printf("\r\n");
        }
        printf(" %f", yt[i]);
    }
    printf("\r\n");
    return 0;
}


static int demo_rk4_solver()
{
    float yt[TSPAN_SIZE] = {0};
    ode_rk4_calcu(yt, dy, 0.5, 0, dy(0, 0), TSPAN_SIZE);
    for(int i = 0; i < TSPAN_SIZE; i++){
        if(i % 10 == 0){
            printf("\r\n");
        }
        printf(" %f", yt[i]);
    }
    printf("\r\n");
    return 0;
}

int demo_rkf45_solver()
{
    float yt[50] = {0};
    ode_rkf45_calcu(yt, dy, 0.5, 0, dy(0, 0), 0.0005, 0.5, 50);
    for(int i = 0; i < TSPAN_SIZE; i++){
        if(i % 10 == 0){
            printf("\r\n");
        }
        printf(" %f", yt[i]);
    }
    printf("\r\n");
    return 0;
}

int cmd_ode_hdl(int argc, char **argv)
{
    if(argc <= 1){
        return -2;
    }

    if(ISARG(argv[1], "euler")){
        demo_euler_solver();
    }else if(ISARG(argv[1], "rk")){
        demo_rk4_solver();
    }else if(ISARG(argv[1], "rkf45")){
        demo_rkf45_solver();
    }else{
        return -1;
    }

    return 0;
}
