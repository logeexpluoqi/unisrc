/**
 * @ Author: luoqi
 * @ Create Time: 2024-03-29 17:20
 * @ Modified by: luoqi
 * @ Modified time: 2024-04-09 18:54
 * @ Description:
 */

#include <string.h>
#include "qbutton.h"

#define QBUTTON_EVENTS_CALLBACK(action)     if(button->callback[action] != 0) { err = button->callback[action](keyval);}

int qbutton_init(QButton *button, QButtonPressDownKeyVal keyval, uint8_t debounce_tick, uint16_t long_tick, uint8_t short_tick, int (*button_read)(void))
{
    memset(button, 0, sizeof(QButton));
    button->press_keyval = keyval;
    button->button_read = button_read;
    button->debounce_tick = debounce_tick;
    button->long_tick = long_tick;
    button->short_tick = short_tick;
    button->state = QBUTTON_ACTION_NONE;
    return 0;
}

int qbutton_events_attach(QButton *button, QButtonAction action, int (*callback)(int))
{
    if(button->callback[action] == 0) {
        button->callback[action] = callback;
        return 0;
    } else {
        return -1;
    }
}

int qbutton_events_detach(QButton *button, QButtonAction action)
{
    if(button->callback[action] != 0) {
        button->callback[action] = 0;
        return 0;
    } else {
        return -1;
    }
}

int qbutton_exec(QButton *button)
{
    int err = 0;
    int keyval = button->button_read();
    if((keyval == button->press_keyval) && !button->isactive) {
        if(button->debounce++ > button->debounce_tick) {
            button->state = QBUTTON_ACTION_PRESS_DOWN;
            button->isactive = 1;
            button->debounce = 0;
        }
    } else {
        button->debounce = 0;
    }

    switch(button->state) {
    case QBUTTON_ACTION_NONE:
        button->isactive = 0;
        button->repeat = 0;
        break;
    case QBUTTON_ACTION_PRESS_DOWN:
        if(keyval == button->press_keyval) {
            QBUTTON_EVENTS_CALLBACK(QBUTTON_ACTION_PRESS_DOWN);
            if(button->ticks++ > button->long_tick) {
                button->state = QBUTTON_ACTION_PRESS_LONG;
            }
        } else {
            button->state = QBUTTON_ACTION_PRESS_UP;
        }
        break;
    case QBUTTON_ACTION_PRESS_UP:
        if(button->ticks++ > button->short_tick) {
            QBUTTON_EVENTS_CALLBACK(QBUTTON_ACTION_PRESS_UP);
            button->state = QBUTTON_ACTION_NONE;
            button->ticks = 0;
        } else {
            button->state = QBUTTON_ACTION_PRESS_REPEAT;
        }
        break;
    case QBUTTON_ACTION_PRESS_REPEAT:
        if(button->ticks++ < button->short_tick) {
            if(button->isactive) {
                button->repeat++;
            }
        } else {
            if(button->repeat == 1) {
                button->state = QBUTTON_ACTION_SIGLE_CLICK;
            } else if(button->repeat == 2) {
                button->state = QBUTTON_ACTION_DOUBLE_CLICK;
            } else if(button->repeat == 3) {
                button->state = QBUTTON_ACTION_THRIBLE_CLICK;
            } else {
                QBUTTON_EVENTS_CALLBACK(QBUTTON_ACTION_PRESS_REPEAT);
                if(button->callback[QBUTTON_ACTION_PRESS_REPEAT] != 0) {
                    err = button->callback[QBUTTON_ACTION_PRESS_REPEAT](button->repeat);
                }
                button->state = QBUTTON_ACTION_NONE;
            }
        }
        button->isactive = 0;
        break;
    case QBUTTON_ACTION_SIGLE_CLICK:
        QBUTTON_EVENTS_CALLBACK(QBUTTON_ACTION_SIGLE_CLICK);
        button->state = QBUTTON_ACTION_NONE;
        button->ticks = 0;
        break;
    case QBUTTON_ACTION_DOUBLE_CLICK:
        QBUTTON_EVENTS_CALLBACK(QBUTTON_ACTION_DOUBLE_CLICK);
        button->state = QBUTTON_ACTION_NONE;
        button->ticks = 0;
        break;
    case QBUTTON_ACTION_THRIBLE_CLICK:
        QBUTTON_EVENTS_CALLBACK(QBUTTON_ACTION_THRIBLE_CLICK);
        button->state = QBUTTON_ACTION_NONE;
        button->ticks = 0;
        break;
    case QBUTTON_ACTION_PRESS_LONG:
        QBUTTON_EVENTS_CALLBACK(QBUTTON_ACTION_PRESS_LONG);
        button->state = QBUTTON_ACTION_NONE;
        button->isactive = 1;
        button->ticks = button->short_tick;
        break;
    default:
        return -1;
    }
    return err;
}
