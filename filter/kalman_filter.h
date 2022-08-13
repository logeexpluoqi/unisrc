/*
 * @Author: luoqi 
 * @Date: 2021-04-27 19:23:50 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-04-28 19:32:46
 */

#ifndef _KALMAN_FILTER_H
#define _KALMAN_FILTER_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct _kalman_filter_1dimension
{
    float x; // state, x(k) = a*x(k-1) + b*u(k)
    float a; // state transfer matrix
    float b; // control matrix
    float h; // observer matrix
    float p; // estimate covariance
    float q; // process covariance
    float r; // observe covariance
    float g; // kalman gain
} KF1DimObj;

int kf_1dim_init(KF1DimObj *kf, float x0, float a, float b, float h, float p0, float q, float r);

float kf_1dim_calcu(KF1DimObj *kf, float z);

typedef struct  _kalman_filter_2dimension
{
    float X[2];
    float A[2][2];
    float B[2];
    float H[2];
    float P[2][2];
    float Q[2];
    float R[2];
    float G[2];
} KF2DimObj;

int kf_2dim_init(KF2DimObj *kf, float X0[2], float A[2][2], float B[2], float H[2], float P0[2][2], float Q[2], float R[2]);

float *kf_2dim_calcu(KF2DimObj *kf, float X[2]);

#ifdef __cplusplus
 }
#endif

#endif
    