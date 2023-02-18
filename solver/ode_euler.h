/*
 * @Author: luoqi 
 * @Date: 2021-09-17 11:13:12 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-28 00:07:49
 */

#ifndef _ODE_EULER_H
#define _ODE_EULER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float (*dy)(float x, float y);

    float y;
    float x;
    float h;
} OdeEuler;

int ode_euler_1st_calcu(float *y, float (*dy)(float _x, float _y), float h, float x0, float y0, int len);

int ode_euler_k_init(OdeEuler *solver, float (*dy)(float _x, float _y), float x0, float y0, float h);

float ode_euler_1st_k_calcu(OdeEuler *solver);

/* prediction-correction formula */
int ode_euler_2st_calcu(float *y, float (*dy)(float _x, float _y), float h, float x0, float y0, int len);

float ode_euler_2st_k_calcu(OdeEuler *solver);

#ifdef __cplusplus
}
#endif

#endif
