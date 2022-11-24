/*
 * @Author: luoqi 
 * @Date: 2022-01-26 14:45:52 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-24 23:44:27
 */

#include "ode_euler.h"

int ode_euler(float *yt, float (*dy)(float t), float h, float y0, float t_start, unsigned int len)
{
    yt[0] = dy(t_start);
    for(unsigned int i = 0; i < (len - 1); i ++){
        yt[i+1] = yt[i] + h * dy(i * h + t_start);
    }
    return 0;
}

int ode_euler_k_init(OdeEuer *solver, float (*dy)(float), float y0, float h)
{
    solver->dy = dy;
    solver->y0 = y0;
    solver->h = h;
    solver->y_k1 = 0;
    solver->k = 0;
    return 0;
}

float ode_euler_k_calcu(OdeEuer *solver)
{
    return (solver->y_k1 + solver->h * solver->dy(solver->k * solver->h));
}
