/*
 * @Author: luoqi 
 * @Date: 2021-10-17 20:28:35 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 16:20:30
 */

#ifndef _QKEY_H
#define _QKEY_H

#include "../../kernel/qlist.h"

typedef enum _qkey_state
{
    QKEY_NO_ACTION,
    QKEY_IS_PRESSED,
    
} QKeyState;

typedef enum _qkey_press_def
{
    QKEY_L_IS_PRESSED = 0,
    QKEY_H_IS_PRESSED = 1
} QKeyPressDef;

typedef struct _qkey_obj
{
    const unsigned char* name;
    int (*callback)(void);
    int (*getkey)(void);
    int key_id;
    QKeyState key_state;
    QKeyPressDef press;
    unsigned int debounce_time;
    unsigned int debounce_cnt;
    unsigned char debounce_start;
    ListObj qkey_internal_list;
} QKeyObj;

void qkey_init(QKeyObj* key,
               const unsigned char* name,
               QKeyPressDef press,
               int (*getkey)(void),
               int (*callback)(void),
               unsigned int debounce_time);

void qkey_exec(void);

void qkey_tick(void);

#endif
