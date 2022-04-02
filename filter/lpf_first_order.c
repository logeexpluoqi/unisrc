/*
 * @Author: luoqi 
 * @Date: 2022-03-16 09:27:04 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-03-16 09:27:33
 */

#include "lpf_first_order.h"

#define LPF_PI  3.1415926

void lpf_first_order_init(LpfFirstOderObj *filter, float alpha)
{
    filter->alpha = alpha;
    filter->fc = 0;
    filter->T = 0;
    filter->u_k1 = 0;
}

float lpf_first_order_calcu(LpfFirstOderObj *filter, float z)
{
    float u_k = filter->alpha * z + (1 - filter->alpha) * filter->u_k1;
    filter->u_k1 = u_k;

    return u_k;
}

void lpf_first_order_fc_set(LpfFirstOderObj *filter, float fc, float T)
{
    filter->fc = fc;
    filter->T = T;
    filter->alpha = (2 * LPF_PI * T * fc) / (1 + 2 * LPF_PI * T * fc);
}
