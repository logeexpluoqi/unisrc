/*
 * @Author: luoqi 
 * @Date: 2021-09-17 11:03:50 
 * @Last Modified by:   luoqi 
 * @Last Modified time: 2021-09-17 11:03:50 
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
