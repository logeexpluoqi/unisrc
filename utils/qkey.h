/*
 * @Author: luoqi 
 * @Date: 2021-10-17 20:28:35 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-10 23:30:05
 */

#ifndef _QKEY_H
#define _QKEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../qlib/qlist.h"

typedef enum _qkey_trig_def {
    QKEY_STATE_LOW = 0,
    QKEY_STATE_HIGH = 1
} QKeyState;

typedef struct _qkey_obj {
    const char *name;

    int (*callback)(void);

    QKeyState (*getkey)(void);

    int id;
    int err;
    QKeyState state;
    unsigned int dtime;     // debounce time
    unsigned int dcnt;      // debounce counter
    unsigned char dstart;   // debounce start
    QList qkey_node;
} QKeyObj;

int qkey_init(QKeyObj *key,
              const char *name,
              QKeyState state,
              QKeyState (*getkey)(void),
              int (*callback)(void),
              unsigned int dtime);

int qkey_exec(void);

int qkey_tick(void);

#ifdef __cplusplus
}
#endif

#endif
