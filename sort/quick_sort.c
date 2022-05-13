/*
 * @Author: luoqi 
 * @Date: 2022-04-23 18:05:44 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-05-01 13:15:54
 */
#include <stdint.h>
#include "quick_sort.h"
#include "sort_comm.h"

static int _quick_short_recu(float *data, int len, int begin, int end);

int quick_sort_recu(float *data, int len) 
{
    return (_quick_short_recu(data, len, 0, len - 1));
}

int _quick_short_recu(float *data, int len, int begin, int end)
{
    int i, j;
    if(begin < end){
            i = begin + 1;
            j = end;
        
        while(i < j) {
            if(data[i] > data[begin]) {
                sort_swap(&data[i], &data[j]);
                j--; 
            } else {  
                i++;
            }
        }
        if(data[i] >= data[begin]) { 
            i--;
        }
        sort_swap(&data[begin], &data[i]);
        _quick_short_recu(data, len, begin, i);
        _quick_short_recu(data, len, j, end);
    }else{
        return 0;
    }
    
    return 0;
}
