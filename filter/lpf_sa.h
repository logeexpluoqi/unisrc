/*
 * @Author: luoqi 
 * @Date: 2022-03-15 09:59:42 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-03-15 10:08:42
 */

#ifndef _LPF_SA_H
#define _LPF_SA_H

#ifdef __cplusplus
 extern "C" {
#endif

#define LPF_SA_USING_LIBC  1

typedef struct _lpf_sa
{
    int wsize;
    int head;
    float sum;
    float *cache;
} LpfSaObj;

void lpf_sa_init(LpfSaObj *filter, int wsize);

float lpf_sa_calcu(LpfSaObj *filter, float z);

int lpf_sa_cache_add(LpfSaObj *filter, float *cache, int wsize);

#ifdef __cplusplus
 }
#endif

#endif
