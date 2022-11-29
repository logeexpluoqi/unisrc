/*
 * @Author: luoqi 
 * @Date: 2022-03-16 09:27:04 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-29 09:54:57
 */

#include "lpf_1st.h"

#define LPF_PI  3.1415926

void lpf_1st_init(Lpf1stObj *filter, float alpha)
{
    filter->alpha = alpha;
    filter->fc = 0;
    filter->ts = 0;
    filter->u_k1 = 0;
}

float lpf_1st_calcu(Lpf1stObj *filter, float xk)
{
    float u_k = filter->alpha * xk + (1 - filter->alpha) * filter->u_k1;
    filter->u_k1 = u_k;

    return u_k;
}

void lpf_1st_fc_set(Lpf1stObj *filter, float fc, float ts)
{
    filter->fc = fc;
    filter->ts = ts;
    filter->alpha = (2 * LPF_PI * ts * fc) / (1 + 2 * LPF_PI * ts * fc);
}
