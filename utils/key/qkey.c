/*
 * @Author: luoqi 
 * @Date: 2021-10-17 20:29:15 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 17:45:01
 */

#include "qkey.h"

static LIST_HEAD(key_list);
static unsigned int key_id_base = 0;

void qkey_init(QKeyObj* key, 
                const unsigned char* name, 
                QKeyTrigDef trig, 
                int (*getkey)(void), 
                int (*callback)(void), 
                unsigned int debounce_time)
{
    key->name = name;
    key->key_id = key_id_base;
    key->getkey = getkey;
    key->callback = callback;
    key->trig = trig;
    key->debounce_start = 0;
    key->key_state = QKEY_NO_ACTION;
    key->debounce_time = debounce_time;
    key->debounce_cnt = debounce_time;
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
        if(key->getkey() == key->trig)
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
            key->debounce_cnt --;
        if(key->debounce_cnt == 0)
        {
            key->key_state = QKEY_IS_PRESSED;
            key->debounce_cnt = key->debounce_time;
        }
    }
}

