/*
 * @Author: luoqi 
 * @Date: 2021-09-17 11:13:12 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-08-13 23:14:28
 */

#ifndef _ODE_EULER_H
#define _ODE_EULER_H

#ifdef __cplusplus
 extern "C" {
#endif

int ode_euler(float *yt, float (*dy)(float t), float h, float y0, float t_start, unsigned int len);

#ifdef __cplusplus
 }
#endif

#endif
