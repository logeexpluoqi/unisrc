/*
 * @Author: luoqi 
 * @Date: 2021-10-17 20:29:15 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-10-17 21:42:55
 */

#include "qkey.h"

static LIST_HEAD(key_list);
static unsigned int key_id_base = 0;

void qkey_init(QKeyObj* key, const unsigned char* name, QKeyPressDef press_def, int (*getkey)(void), int (*callback)(void), unsigned int debounce_time)
{
    key->name = name;
    key->key_id = key_id_base;
    key->getkey = getkey;
    key->callback = callback;
    key->press_def = press_def;
    key->debounce_start = 0;
    key->key_state = QKEY_NO_ACTION;
    key->debounce_time = debounce_time;
    list_insert_before(&key->qkey_internal_list, &key_list);
    key_id_base ++;
}

void qkey_exec()
{
    ListObj* node;
    QKeyObj* key;
    int key_state;

    list_for_each(node, &key_list)
    {
        key = list_entry(node, QKeyObj, qkey_internal_list);
        if(key->getkey() == key->press_def)
            key->debounce_start = 1;

        if(key->key_state == QKEY_IS_PRESSED)
        {
            key->callback();
            key->key_state = QKEY_NO_ACTION;
            key->debounce_start = 0;
        }
    }
}

void qkey_tick()
{
    ListObj* node;
    QKeyObj* key;
    
    list_for_each(node, &key_list)
    {
        key = list_entry(node, QKeyObj, qkey_internal_list);
        if(key->debounce_start == 1)
            key->debounce_time --;
        if(key->debounce_time == 0)
        {
            key->key_state = QKEY_IS_PRESSED;
        }
    }
}

