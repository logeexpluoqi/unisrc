/*
 * @Author: luoqi 
 * @Date: 2021-09-17 11:02:33 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 14:24:34
 */

#ifndef _QMATH_H
#define _QMATH_H

#ifdef __cplusplus
extern "C" {
#endif

#define QMATH_INF  (1.0f/0.0)
#define QMATH_IINF (-1.0f/0.0)
#define QMATH_NAN  (0.0f/0.0)

float qabs(float x);

float qsign(float x);

float qmax(float a, float b);

float qmax_arr(float *data, int len);

float qmin(float a, float b);

float qmin_arr(float *data, int len);

float qfloor(float x);

float qceil(float x);

float qmean(float *data, int len);

#define QPOW_ACCURACY   0.01
float qpow(float a, float b);

#ifdef __cplusplus
}
#endif

#endif
