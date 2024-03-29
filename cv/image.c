/*
 * @Author: luoqi 
 * @Date: 2023-02-21 21:27:56 
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-02-21 21:29:43
 */

#include "image.h"

#if IMAGE_USING_STDLIB
#include <stdlib.h>
#endif

int image_set(ImageObj *image, char **cache, int rows, int cols)
{
    image->img = cache;
    image->rows = rows;
    image->cols = cols;
    return 0;
}

#if IMAGE_USING_STDLIB

ImageObj *image_creat(int rows, int cols)
{

    ImageObj *image = malloc(sizeof(ImageObj));
    char *cache = malloc(sizeof(int) * rows * cols);
    image->img = cache;
    return image;
}

void image_destroy(ImageObj *image)
{
    free(image->img);
    free(image);
}

#endif
