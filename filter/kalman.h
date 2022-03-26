/*
 * @Author: luoqi 
 * @Date: 2021-04-27 19:23:50 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-04-28 19:32:46
 */

#ifndef _KALMAN_H
#define _KALMAN_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct _kalman_filter
{
    float x;    // state
    float F;    // x(k) = A * x(k-1) + w(k), w(k)~N(0,Q)
    float H;    // z(k) = H * x(k) + v(k), v(k)~N(0,R)
    float Q;    // process/predict convariance
    float R;    // measure noise convariance
    float P;    // estimate error convariance
    float Kg;   // kalman gain
} KalmanFilter1DObj;

#ifdef __cplusplus
 }
#endif

#endif
