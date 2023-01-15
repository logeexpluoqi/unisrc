/*
 * @Author: luoqi 
 * @Date: 2022-11-08 20:38:50 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-08 20:42:54
 */

#ifndef _QUI_H
#define _QUI_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int size_x;
    int size_y;
} QuiObj;

int qui_init(QuiObj *ui, int size_x, int size_y);


#ifdef __cplusplus
}
#endif

#endif
