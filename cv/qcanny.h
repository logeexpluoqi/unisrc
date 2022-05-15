/*
 * @Author: luoqi 
 * @Date: 2022-04-23 17:10:44 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 18:13:56
 */

#ifndef _QCANNY_H
#define _QCANNY_H
#ifdef __cplusplus
 extern "C" {
#endif

#define QCANNY_IMG_XSIZE    100
#define QCANNY_IMG_YSIZE    100

int qcanny(int **img, float th_low, float th_high, int compensation);

#ifdef __cplusplus
 }
#endif

#endif
