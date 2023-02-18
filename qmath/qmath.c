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

float qmax_arr(float* data, int len)
{
    float max = data[0];
    int i;
    for(i = 1; i < len; i++) {
        if(max < data[i]) {
            max = data[i];
        } else {
            continue;
        }
    }
    return max;
}

float qmin_arr(float* data, int len)
{

    float min = data[0];
    int i;
    for(i = 1; i < len; i++) {
        if(min > data[i]) {
            min = data[i];
        } else {
            continue;
        }
    }
    return min;
}

float qmean(float* data, int len)
{
    int i;
    float x = 0;
    for(i = 0; i < len; i++) {
        x += data[i];
    }
    return (x / len);
}

static float pow_i(float t, int n) {
    float x = 1.0;
    for(int i = 0; i < n; i++) {
        x = x * t;
    }

    return x;
}

static float pow_d(float b, int n) {
    float x = 1;
    for(int i = 1; i <= n; i++) {
        x = x * (b - i + 1) / i;
    }

    return x;
}

float qpow(float a, float b) {
    int p = 0;
    float t, x = 1.0;

    if((a == 0) && (b > 0)) {
        return 0;
    } else if((a == 0) && (b <= 0)) {
        return 0xffffffff;
    } else if((a < 0) && (!(b - (int)b < 0.0001) || (b - (int)b > 0.999))) {
        return 0xffffffff;
    }

    if((a >= 0) && (a <= 2)) {
        t = a - 1;
        for(int i = 1; i < (int)(1.0 / QPOW_ACCURACY); i++) {
            x = x + pow_i(t, i) * pow_d(b, i);
        }
        return x;
    } else if(a > 2) {

        while(a > 2) {
            a = a / 2;
            p++;
        }

        return qpow(a, b) * qpow(2, b * p);
    } else {
        if(((int)b % 2) == 0) {
            return qpow(-a, b);
        } else {
            return -qpow(-a, b);
        }
    }
}
