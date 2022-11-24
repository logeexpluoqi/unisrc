/*
 * @Author: luoqi 
 * @Date: 2021-09-17 11:14:45 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-09-17 11:15:15
 */

#ifndef _ODE_RK_H
#define _ODE_RK_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct ode_rk
{
    int order;
    float (*dy)(float k);
    float y_k1;
} OdeRKx;

int ode_rkx_k_init(OdeRKx *solver, float (*dy)(float k), int order);

float ode_rkx_k_calcu(OdeRKx *solver);

#ifdef __cplusplus
 }
#endif

#endif
