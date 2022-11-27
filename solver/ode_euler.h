/*
 * @Author: luoqi 
 * @Date: 2021-09-17 11:13:12 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-27 21:51:57
 */

#ifndef _ODE_EULER_H
#define _ODE_EULER_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct 
{
    float (*dy)(float x, float y);
    float y;
    float x;
    float h;
} OdeEuer;

int ode_euler_1st_calcu(float *y, float (*dy)(float _x, float _y), float h, float y0, float t0, int len);

int ode_euler_k_init(OdeEuer *solver, float (*dy)(float _x, float _y), float y0, float h);

float ode_euler_1st_k_calcu(OdeEuer *solver);

/* prediction-correction formula */

int ode_euler_2st_calcu(float *y, float (*dy)(float _x, float _y), float h, float y0, float t0, int len);

int ode_euler_2st_k_init(OdeEuer *solver, float (*dy)(float _x, float _y), float y0, float h);

float ode_euler_2st_k_calcu(OdeEuer *solver);

#ifdef __cplusplus
 }
#endif

#endif
