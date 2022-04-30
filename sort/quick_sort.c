/*
 * @Author: luoqi 
 * @Date: 2022-04-23 18:05:44 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 18:36:30
 */

#include "quick_sort.h"
#include "sort_comm.h"

int quick_sort(float *data, unsigned int len, unsigned int begin, unsigned int end) 
{  
    unsigned int i, j;  
  
    if(begin < end) {  
        i = begin + 1; 
        j = end - 1;
            
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
        quick_sort(data, len, begin, i);  
        quick_sort(data, len, j, end - 1);  
    }
    return 0;
}
