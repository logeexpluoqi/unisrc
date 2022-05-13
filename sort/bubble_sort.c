/*
 * @Author: luoqi 
 * @Date: 2022-05-13 21:52:54 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-05-13 22:15:37
 */

#include "sort_comm.h"

int bubble_sort(float *data, int len)
{
    int i, j;
    for(j = len - 1; j > 0; j--){
        for(i = 0; i < j; i ++){
            if(data[i] > data[i + 1]){
                sort_swap(&data[i], &data[i+1]);
            }else{
                continue;
            }
        }
    }
}
