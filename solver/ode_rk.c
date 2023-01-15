/*
 * @Author: luoqi
 * @Date: 2021-09-17 11:15:21
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-27 22:49:14
 */

#include "ode_rk.h"

int ode_rk4_init(OdeRKx *solver, float (*dy)(float _x, float _y), float h, float x0, float y0)
{
    solver->dy = dy;
    solver->h = h;
    solver->y = y0;
    solver->x = x0;
    solver->tolerance = 0;
    return 0;
}

int ode_rk4_calcu(float *y, float (*dy)(float _x, float _y), float h, float x0, float y0, int len)
{
    float xn, k1, k2, k3, k4;
    if (len < 0) {
        return -1;
    }
    y[0] = y0;
    for (int i = 0; i < (len - 1); i++) {
        xn = (float) i * h + x0;
        k1 = dy(xn, y[i]);
        k2 = dy(xn + (h / 2), y[i] + (h / 2) * k1);
        k3 = dy(xn + (h / 2), y[i] + (h / 2) * k2);
        k4 = dy(xn + h, y[i] + h * k3);
        y[i + 1] = y[i] + (h / 6) * (k1 + (2 * k2) + (2 * k3) + k4);
    }
    return 0;
}

float ode_rk4_k_calcu(OdeRKx *solver)
{
    float k1 = solver->dy(solver->x, solver->y);
    float k2 = solver->dy(solver->x + (solver->h / 2), solver->y + (solver->h / 2) * k1);
    float k3 = solver->dy(solver->x + (solver->h / 2), solver->y + (solver->h / 2) * k2);
    float k4 = solver->dy(solver->x + solver->h, solver->y + solver->h * k3);
    solver->y = solver->y + (solver->h / 6) * (k1 + (2 * k2) + (2 * k3) + k4);
    solver->x = solver->x + solver->h;
    return solver->y;
}

int ode_rk45_calcu(float *y, float (*dy)(float _x, float _y), float h, float x0, float y0, int len)
{

    return 0;
}

float ode_rk45_k_calcu(OdeRKx *solver)
{

    return solver->y;
}
