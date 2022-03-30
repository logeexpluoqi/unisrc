/*
 * @Author: luoqi 
 * @Date: 2022-03-15 10:01:18 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-03-15 15:52:19
 */

#include "sliding_average_filter.h"
#ifdef SLIDING_AVERAGE_FILTER_USEING_STDLIB
 #include <stdlib.h>
 #include <string.h>
#endif


void sliding_average_filter_init(SlidAveFilterObj *filter, int w_size)
{
    #ifdef SLIDING_AVERAGE_FILTER_USEING_STDLIB
     filter->cache = (float*)malloc(sizeof(float) * w_size);
     memset(filter->cache, 0, sizeof(float) * w_size);
    #endif

    filter->w_size = w_size;
    filter->sum = 0;
    filter->head = 0;
}

float sliding_average_filter_calcu(SlidAveFilterObj *filter, float k)
{
    filter->cache[filter->head] = k;
    filter->head = (filter->head + 1) % filter->w_size;
    filter->sum = filter->sum + k - filter->cache[filter->head];

    return (filter->sum / (float)filter->w_size);
}

int sliding_average_filter_cache_add(SlidAveFilterObj *filter, float *cache, int w_size)
{
    filter->cache = cache;
    filter->w_size = w_size;
    return 0;
}
