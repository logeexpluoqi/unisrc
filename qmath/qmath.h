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

float qabs(float x);

float qsign(float x);

float qmax(float a, float b);

float qmax_arr(float *data, int len);

float qmin(float a, float b);

float qmin_arr(float *data, int len);

float qfloor(float x);

float qceil(float x);

#ifdef __cplusplus
 }
#endif

#endif
