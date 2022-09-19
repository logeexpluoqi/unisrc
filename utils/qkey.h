/*
 * @Author: luoqi 
 * @Date: 2021-10-17 20:28:35 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-08-14 00:17:12
 */

#ifndef _QKEY_H
#define _QKEY_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "../frame/qlist.h"

typedef enum _qkey_state
{
    QKEY_NO_ACTION,
    QKEY_IS_PRESSED,
    
} QKeyState;

typedef enum _qkey_trig_def
{
    QKEY_L_IS_TRIG = 0,
    QKEY_H_IS_TRIG = 1
} QKeyTrigDef;

typedef struct _qkey_obj
{
    const unsigned char* name;
    int (*callback)(void);
    int (*getkey)(void);
    int id;
    QKeyState state;
    QKeyTrigDef trig;
    unsigned int dtime;     // debounce time
    unsigned int dcnt;      // debounce counter
    unsigned char dstart;   // debounce start
    ListObj qkey_node;
} QKeyObj;

void qkey_init(QKeyObj* key,
               const unsigned char* name,
               QKeyTrigDef trig,
               int (*getkey)(void),
               int (*callback)(void),
               unsigned int dtime);

void qkey_exec(void);

void qkey_tick(void);

#ifdef __cplusplus
 }
#endif

#endif
