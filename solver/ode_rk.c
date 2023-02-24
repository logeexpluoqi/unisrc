/*
 * @Author: luoqi
 * @Date: 2021-09-17 11:15:21
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-02-21 21:20:48
 */

#include "ode_rk.h"

static float qabs(float x)
{
    return ((x >= 0) ? x : -x);
}

int ode_rk4_init(OdeRKx* solver, float (*dy)(float _x, float _y), float h, float x0, float y0)
{
    solver->dy = dy;
    solver->h = h;
    solver->y = y0;
    solver->x = x0;
    solver->tol = 0;
    return 0;
}

int ode_rk4_calcu(float* y, float (*dy)(float _x, float _y), float h, float x0, float y0, int len)
{
    float xn, k1, k2, k3, k4;
    if(len < 1) {
        return -1;
    }
    y[0] = y0;
    for(int i = 0; i < (len - 1); i++) {
        xn = (float)i * h + x0;
        k1 = dy(xn, y[i]);
        k2 = dy(xn + (h / 2), y[i] + (h / 2) * k1);
        k3 = dy(xn + (h / 2), y[i] + (h / 2) * k2);
        k4 = dy(xn + h, y[i] + h * k3);
        y[i + 1] = y[i] + (h / 6) * (k1 + (2 * k2) + (2 * k3) + k4);
    }
    return 0;
}

float ode_rk4_kcalcu(OdeRKx* solver)
{
    float k1 = solver->dy(solver->x, solver->y);
    float k2 = solver->dy(solver->x + (solver->h / 2), solver->y + (solver->h / 2) * k1);
    float k3 = solver->dy(solver->x + (solver->h / 2), solver->y + (solver->h / 2) * k2);
    float k4 = solver->dy(solver->x + solver->h, solver->y + solver->h * k3);
    solver->y = solver->y + (solver->h / 6) * (k1 + (2 * k2) + (2 * k3) + k4);
    solver->x = solver->x + solver->h;
    return solver->y;
}

int ode_rkf45_init(OdeRKx* solver, float (*dy)(float _x, float _y), float h0, float x0, float y0, float tol)
{
    solver->dy = dy;
    solver->h = h0;
    solver->x = x0;
    solver->y = y0;
    solver->tol = tol;
    return 0;
}

static float pow_i(float t, int n) {
    float x = 1.0;
    for(int i = 0; i < n; i++) {
        x = x * t;
    }

    return x;
}

#define RK_POW_ACCURACY    0.01
static float pow_d(float b, int n) {
    float x = 1;
    for(int i = 1; i <= n; i++) {
        x = x * (b - i + 1) / i;
    }

    return x;
}

static float rk_pow(float a, float b) {
    int p = 0;
    float t, x = 1.0;

    if((a == 0) && (b > 0)) {
        return 0;
    } else if((a == 0) && (b <= 0)) {
        return 0xffffffff;
    } else if((a < 0) && (!(b - (int)b < 0.0001) || (b - (int)b > 0.999))) {
        return 0xffffffff;
    }

    if((a >= 0) && (a <= 2)) {
        t = a - 1;
        for(int i = 1; i < (int)(1.0 / RK_POW_ACCURACY); i++) {
            x = x + pow_i(t, i) * pow_d(b, i);
        }
        return x;
    } else if(a > 2) {
        while(a > 2) {
            a = a / 2;
            p++;
        }
        return qpow(a, b) * qpow(2, b * p);
    } else {
        if(((int)b % 2) == 0) {
            return qpow(-a, b);
        } else {
            return -qpow(-a, b);
        }
    }
}

int ode_rkf45_calcu(float* y, float (*dy)(float _x, float _y), float h0, float x0, float y0, float tol, float max_h, int len)
{
    float _h, k1, k2, k3, k4, k5, k6, w, z, xn;
    int _bingo;
    if(len < 1) {
        return -1;
    }
    y[0] = y0;
    _h = h0;
    for(int i = 0; i < len - 1; i++) {
        _bingo = 0;
        while(!_bingo) {
            xn = (float)i * _h + x0;
            k1 = dy(xn, y[i]);
            k2 = dy(xn + 0.25 * _h, y[i] + 0.25 * _h * k1);
            k3 = dy(xn + 0.375 * _h, y[i] + 0.09375 * _h * k1 + 0.28125 * _h * k2);
            k4 = dy(xn + (12.f / 13.f) * _h, y[i] + (1932.f / 2197.f) * _h * k1 - (7200.f / 2197.f) * _h * k2 + (7296.f / 2197.f) * _h * k3);
            k5 = dy(xn + _h, y[i] + (439.f / 216.f) * _h * k1 - 8.f * _h * k2 + (3680.f / 513.f) * _h * k3 - (845.f / 4104.f) * _h * k4);
            k6 = dy(xn + 0.5 * _h, y[i] - (8.f / 27.f) * _h * k1 + 2.f * _h * k2 - (3544.f / 2565.f) * _h * k3 + (1859.f / 4104.f) * _h * k4 - 0.275 * _h * k5);
            w = y[i] + _h * ((25.f / 216.f) * k1 + (1408.f / 2565.f) * k3 + (2197.f / 4104.f) * k4 - 0.2 * k5);
            z = y[i] + _h * ((16.f / 135.f) * k1 + (6656.f / 12825.f) * k3 + (28561.f / 56430.f) * k4 - 0.18 * k5 + (2.f / 55.f) * k6);
            if((qabs(z - w) / qabs(w)) < tol) {
                y[i + 1] = z;
                xn = xn + _h;
            #if IS_USING_SIMPLE_RKF45
                _h = _h * 2;
            #else
                _h = _h + 0.8 * rk_pow(solver->tol * qabs(w) / (z - w), 0.2) * _h;
            #endif
                _bingo = 1;
            } else {
            #if IS_USING_SIMPLE_RKF45
                _h = _h / 2;
            #else
                _h = 0.8 * rk_pow(solver->tol * qabs(w) / (z - w), 0.2) * _h;
            #endif
            }
            if(_h > max_h){
                _h = max_h;
            }
        }
    }
    return 0;
}

float ode_rkf45_kcalcu(OdeRKx* solver, float max_h)
{
    float _h, k1, k2, k3, k4, k5, k6, w, z;
    int _bingo;
    while(!_bingo){
        _bingo = 0;
        _h = solver->h;
        k1 = solver->dy(solver->x, solver->y);
        k2 = solver->dy(solver->x + 0.25 * _h, solver->y + 0.25 * _h * k1);
        k3 = solver->dy(solver->x + 0.375 * _h, solver->y + 0.09375 * _h * k1 + 0.28125 * _h * k2);
        k4 = solver->dy(solver->x + (12.f / 13.f) * _h, solver->y + (1932.f / 2197.f) * _h * k1 - (7200.f / 2197.f) * _h * k2 + (7296.f / 2197.f) * _h * k3);
        k5 = solver->dy(solver->x + _h, solver->y + (439.f / 216.f) * _h * k1 - 8.f * _h * k2 + (3680.f / 513.f) * _h * k3 - (845.f / 4104.f) * _h * k4);
        k6 = solver->dy(solver->x + 0.5 * _h, solver->y - (8.f / 27.f) * _h * k1 + 2.f * _h * k2 - (3544.f / 2565.f) * _h * k3 + (1859.f / 4104.f) * _h * k4 - 0.275 * _h * k5);
        w = solver->y + _h * ((25.f / 216.f) * k1 + (1408.f / 2565.f) * k3 + (2197.f / 4104.f) * k4 - 0.2 * k5);
        z = solver->y + _h * ((16.f / 135.f) * k1 + (6656.f / 12825.f) * k3 + (28561.f / 56430.f) * k4 - 0.18 * k5 + (2.f / 55.f) * k6);
        if((qabs(z - w) / qabs(w)) < solver->tol) {
            solver->y = z;
            solver->x = solver->x + _h;
        #if IS_USING_SIMPLE_RKF45
            _h = _h * 2;
        #else
            _h = _h + 0.8 * rk_pow(solver->tolerance * qabs(w) / (z - w), 0.2) * _h;
        #endif
            _bingo = 1;
        } else {
        #if IS_USING_SIMPLE_RKF45
            _h = _h / 2;
        #else
            _h = 0.8 * rk_pow(solver->tolerance * qabs(w) / (z - w), 0.2) * _h;
        #endif
        }
        if(_h > max_h){
            _h = max_h;
        }
        solver->h = _h;
    }

    return solver->y;
}
