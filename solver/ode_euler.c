/*
 * @Author: luoqi 
 * @Date: 2022-01-26 14:45:52 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-08-13 23:16:52
 */

#include "ode_euler.h"

int ode_euler(float *yt, float (*dy)(float t), float h, float y0, float t_start, unsigned int len)
{
    yt[0] = dy(t_start);
    for(unsigned int i = 0; i < (len - 1); i ++){
        yt[i+1] = yt[i] + h * dy(i * h + t_start);
    }
    return 0;
}
