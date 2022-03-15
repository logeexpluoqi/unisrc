/*
 * @Author: luoqi 
 * @Date: 2022-03-15 10:01:18 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-03-15 15:52:19
 */

#include <stdlib.h>
#include <string.h>
#include "sliding_average_filter.h"

void sliding_average_filter_init(SlidAveFilterObj *filter, int w_size)
{
    filter->cache = (float*)malloc(sizeof(float) * w_size);
    memset(filter->cache, 0, sizeof(float) * w_size);
    filter->w_size = w_size;
    filter->sum = 0;
    filter->head = 0;
}

float sliding_average_filter(SlidAveFilterObj *filter, float k)
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
