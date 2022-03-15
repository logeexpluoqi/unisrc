/*
 * @Author: luoqi 
 * @Date: 2022-03-15 09:59:42 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-03-15 10:08:42
 */

#ifndef _SLIDING_AVERAGE_FILTER_H
#define _SLIDING_AVERAGE_FILTER_H

typedef struct _sliding_average_filter
{
    int w_size;
    int head;
    float sum;
    float counter;
    float *cache;
} SlidAveFilterObj;

void sliding_average_filter_init(SlidAveFilterObj *filter, int w_size);

float sliding_average_filter(SlidAveFilterObj *filter, float k);

int sliding_average_filter_cache_add(SlidAveFilterObj *filter, float *cache, int w_size);

#endif
