/*
 * @Author: luoqi 
 * @Date: 2023-02-21 21:23:50 
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-02-21 21:30:05
 */

#ifndef _IMAGE_H
#define _IMAGE_H
#ifdef __cplusplus
 extern "C" {
#endif

#define IMAGE_USING_STDLIB      1

typedef struct image{
    char **img;
    int rows;
    int cols;
} ImageObj;

int image_set(ImageObj *image, char **cache, int rows, int cols);

#if IMAGE_USING_STDLIB

ImageObj image_creat(int rows, int cols);

void image_destroy(ImageObj *image);

#endif

#ifdef __cplusplus
 }
#endif

#endif
