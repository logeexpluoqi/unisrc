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
    float x_k;  // state
    float p_k;  // estimate covariance
    float p_k1; // last estimate covariance
    float q;    // process covariance
    float r;    // observe covariance
    float kg;   // kalman gain
} KF1DimObj;

void kf_1dim_init(KF1DimObj* kf, float q, float r);

float kf_1dim_calcu(KF1DimObj* kf, float k);

#ifdef __cplusplus
 }
#endif

#endif
