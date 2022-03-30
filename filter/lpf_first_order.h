/*
 * @Author: luoqi 
 * @Date: 2022-03-16 09:26:15 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-03-16 09:34:06
 */

#ifndef _LPF_FIRST_ORDER_H
#define _LPF_FIRST_ORDER_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct _lpf_first_order
{
    float fc;   // cut-off frequency
    float uk1;
    float alpha; // filter coefficient
    float T;     // samping period
}LpfFirstOderObj;

void lpf_first_order_init(LpfFirstOderObj *filter, float alpha);

float lpf_first_order_calcu(LpfFirstOderObj *filter, float k);

void lpf_first_order_fc_set(LpfFirstOderObj *filter, float fc, float T);

#ifdef __cplusplus
 }
#endif

#endif
