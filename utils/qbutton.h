/**
 * @ Author: luoqi
 * @ Create Time: 2024-03-29 17:20
 * @ Modified by: luoqi
 * @ Modified time: 2024-11-05 18:01
 * @ Description:
 */

#ifndef _QBUTTON_H
#define _QBUTTON_H

#include <stdint.h>

typedef enum {
    QBUTTON_ACTION_PRESS_DOWN = 0x00,
    QBUTTON_ACTION_PRESS_UP,
    QBUTTON_ACTION_PRESS_REPEAT,
    QBUTTON_ACTION_SIGLE_CLICK,
    QBUTTON_ACTION_DOUBLE_CLICK,
    QBUTTON_ACTION_THRIBLE_CLICK,
    QBUTTON_ACTION_PRESS_LONG,
    QBUTTON_ACTION_NONE,
} QButtonAction;

typedef enum {
    QBUTTON_PRESS_DOWN_KEYVAL_LOW = 0x00,
    QBUTTON_PRESS_DOWN_KEYVAL_HIGH = 0x01
} QButtonPressDownKeyVal;

typedef int (*QButtonEvent)(int);
typedef struct {
    uint8_t   press_keyval;
    uint16_t  ticks;
    uint8_t   debounce;
    uint8_t   state;
    uint8_t   isactive : 1;
    uint8_t   islongpress : 1;
    uint8_t   repeat : 6;
    uint8_t   debounce_tick;
    uint16_t  long_tick;        // long press time threshold
    uint8_t   short_tick;       // repeat press time threshold

    int (*button_read)(void);

    QButtonEvent callback[7];
} QButton;

int qbutton_init(QButton *button, QButtonPressDownKeyVal keyval, uint8_t debounce_tick, uint16_t long_tick, uint8_t short_tick, int (*button_read)(void));

int qbutton_events_attach(QButton *button, QButtonAction action, int (*callback)(int keyval));

int qbutton_events_detach(QButton *button, QButtonAction action);
 
int qbutton_exec(QButton *button);

#endif
