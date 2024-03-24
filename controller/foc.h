/**
 * @Author: luoqi 
 * @Date: 2023-05-22 01:14:18 
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-05-22 01:15:37
 */

#ifndef _FOC_H
#define _FOC_H

#ifdef __cplusplus
extern "C" {
#endif 

#define FOC_NAN  (0.0/0.00

int foc_clark(float ia, float ib, float ic, float *ialpha, float *ibeta);

int foc_iclark(float ialpha, float ibeta, float *ia, float *ib, float *ic);

int foc_park(float ialpha, float ibeta, float *id, float *iq);

int foc_ipark(float ud, float uq, float *ualpha, float *ubeta);

typedef struct foc
{
    
} FocObj;


#ifdef __cpluscplus
 }
#endif
#endif
