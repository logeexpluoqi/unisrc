/*
 * @Author: luoqi 
 * @Date: 2022-01-26 14:45:52 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-27 22:32:55
 */

#include "ode_euler.h"

int ode_euler_1st_calcu(float *y, float (*dy)(float _x, float _y), float h, float x0, float y0, int len)
{
    float xn;
    if(len < 0){
        return -1;
    }
    y[0] = y0;
    for(int i = 0; i < (len - 1); i ++){
        xn = i * h + x0;
        y[i+1] = y[i] + h * dy(xn, y[i]);
    }
    return 0;
}

int ode_euler_k_init(OdeEuer *solver, float (*dy)(float _x, float _y), float h, float x0, float y0)
{
    solver->dy = dy;
    solver->h = h;
    solver->y = y0;
    solver->x = x0;
    return 0;
}

float ode_euler_1st_k_calcu(OdeEuer *solver)
{
    float y = solver->y + solver->h * solver->dy(solver->x, solver->y);
    solver->y = y;
    solver->x = solver->x + solver->h;
    return y;
}

int ode_euler_2st_calcu(float *y, float (*dy)(float _x, float _y), float h, float x0, float y0, int len)
{
    float y_hat;
    float xn;
    if(len < 0){
        return -1;
    }
    y[0] = y0;
    for(int i = 0; i < (len - 1); i ++){
        xn = i * h + x0;
        y_hat = y[i] + h * dy(xn, y[i]);
        y[i+1] = y[i] + (h / 2) * (dy(xn, y[i]) + dy(xn + h, y_hat));
    }
    return 0;
}

float ode_euler_2st_k_calcu(OdeEuer *solver)
{
    float y_hat = solver->y + solver->h * solver->dy(solver->x, solver->y);
    float y = solver->y + (solver->h / 2) * (solver->dy(solver->x, solver->y) + solver->dy(solver->x + solver->h, y_hat));
    solver->y = y;
    solver->x = solver->x + solver->h;
    return y;
}
