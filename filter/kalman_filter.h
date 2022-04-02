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

void kf_1dim_init(KF1DimObj* kf, float x_0, float a, float b, float h, float p_0, float q, float r);

float kf_1dim_calcu(KF1DimObj* kf, float z);

#ifdef __cplusplus
 }
#endif

#endif
