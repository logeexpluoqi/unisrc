/*
 * @Author: luoqi 
 * @Date: 2021-04-27 19:24:21 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-04-27 19:25:22
 */

#include "kalman_filter.h"

void kf_1dim_init(KF1DimObj* kf, float x_0, float a, float b, float h, float p_0, float q, float r)
{
    kf->x = x_0;
    kf->a = a;
    kf->b = b;
    kf->h = h;
    kf->p = p_0;
    kf->q = q;
    kf->r = r;
    kf->g = 0;
}

float kf_1dim_calcu(KF1DimObj* kf, float z)
{
    /* predict, time update */
    kf->x = kf->a * kf->x + kf->b * z;
    kf->p = kf->a * kf->p * kf->a + kf->q;

    /* correct, measurement update */
    kf->g = kf->p * kf->h / (kf->h * kf->p * kf->h + kf->r);
    kf->x = kf->x + kf->g * (z - kf->h * kf->x);
    kf->p = (1 - kf->g * kf->h) * kf->p;

    return kf->x;
}
