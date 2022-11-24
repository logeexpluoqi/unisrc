/*
 * @Author: luoqi 
 * @Date: 2021-09-17 11:13:12 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-24 23:43:57
 */

#ifndef _ODE_EULER_H
#define _ODE_EULER_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct 
{
    float (*dy)(float k);
    float y_k1;
    float y0;
    float k;
    float h;
} OdeEuer;

int ode_euler(float *yt, float (*dy)(float t), float h, float y0, float t_start, unsigned int len);

int ode_euler_k_init(OdeEuer *solver, float (*dy)(float), float y0, float h);

float ode_euler_k_calcu(OdeEuer *solver);

#ifdef __cplusplus
 }
#endif

#endif
