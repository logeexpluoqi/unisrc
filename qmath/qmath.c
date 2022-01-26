/*
 * @Author: luoqi 
 * @Date: 2021-09-17 11:03:50 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 14:26:20
 */

#include "qmath.h"


float qabs(float x)
{
    return (x >= 0.0 ? x : -x);
}


float qsign(float x)
{
    return (x >= 0.0 ? 1.0 : -1.0);
}

float qmax(float a, float b)
{
    if(a >= b)
        return a;
    else 
        return b;
}

float qmin(float a, float b)
{
    if(a <= b)
        return a;
    else
        return b;
}

float qfloor(float x)
{
    return (float)((int)(x));
}

float qceil(float x)
{
    return (float)((int)(x + 1));
}
