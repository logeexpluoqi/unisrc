/**
 * @Author: luoqi 
 * @Date: 2023-05-22 01:13:58 
 * @Last Modified by:   luoqi 
 * @Last Modified time: 2023-05-22 01:13:58 
 */

#include "foc.h"

int foc_clark(float ia, float ib, float ic, float *ialpha, float *ibeta)
{
    *ialpha = (2.f / 3.f) * (ia - 0.5f * ib - 0.5f * ic);
    *ibeta = 
}

int foc_iclark(float ialpha, float ibeta, float *ia, float *ib, float *ic);

int foc_park(float ialpha, float ibeta, float *id, float *iq);

int foc_ipark(float ud, float uq, float *ualpha, float *ubeta);

float sqrt(float x)
{
    if (x < 0.f) {
        return FOC_NAN;
    }
    float y = 0.f;
    float e = 0.5f;
    while (e > 0.001f) {
        y = (x / y + y) * 0.5f;
        e = x - y * y;
    }
    return y;
}
