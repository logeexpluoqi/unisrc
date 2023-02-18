/*
 * @Author: luoqi 
 * @Date: 2021-04-27 19:24:21 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-04-27 19:25:22
 */

#include "kf.h"

int kf_1dim_init(KF1DimObj* kf, float x0, float a, float b, float h, float p0, float q, float r)
{
    kf->x = x0;
    kf->a = a;
    kf->b = b;
    kf->h = h;
    kf->p = p0;
    kf->q = q;
    kf->r = r;
    kf->g = 0;

    return 0;
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

int kf_2dim_init(KF2DimObj *kf, float X0[2], float A[2][2], float B[2], float H[2], float P0[2][2], float Q[2], float R[2])
{
    kf->X[0]    = X0[0];
    kf->X[1]    = X0[1];
    kf->A[0][0] = A[0][0];
    kf->A[0][1] = A[0][1];
    kf->A[1][0] = A[1][0];
    kf->A[1][1] = A[1][1];
    kf->B[0]    = B[0];
    kf->B[1]    = B[1];
    kf->H[0]    = H[0];
    kf->H[1]    = H[1];
    kf->P[0][0] = P0[0][0];
    kf->P[0][1] = P0[0][1];
    kf->P[1][0] = P0[1][0];
    kf->P[1][1] = P0[1][1];
    kf->Q[0]    = Q[0];
    kf->Q[1]    = Q[1];
    kf->R[0]    = R[0];
    kf->R[1]    = R[1];
    kf->G[0]    = 0;
    kf->G[1]    = 0;

    return 0;
}

float *kf_2dim_calcu(KF2DimObj *kf, float X[2])
{

}
