/*
 * @Author: luoqi 
 * @Date: 2021-09-17 11:14:45 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-28 11:17:25
 */

#ifndef _ODE_RK_H
#define _ODE_RK_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float (*dy)(float x, float y);

    float y;
    float x;
    float h;
    float tol;
} OdeRKx;


int ode_rk4_calcu(float *y, float (*dy)(float _x, float _y), float h, float x0, float y0, int len);

int ode_rk4_init(OdeRKx *solver, float (*dy)(float _x, float _y), float h, float x0, float y0);

float ode_rk4_kcalcu(OdeRKx *solver);

#define IS_USING_SIMPLE_RKF45    1

/* Runge-Kutta-Fehlberg method */
int ode_rkf45_init(OdeRKx *solver, float (*dy)(float _x, float _y), float h0, float x0, float y0, float tol);

int ode_rkf45_calcu(float *y, float (*dy)(float _x, float _y), float h0, float x0, float y0, float tol, float max_h, int len);

float ode_rkf45_kcalcu(OdeRKx *solver, float max_h);

#ifdef __cplusplus
}
#endif

#endif
