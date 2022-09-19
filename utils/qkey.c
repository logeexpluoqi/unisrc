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
                unsigned int dtime)
{
    key->name = name;
    key->id = key_id_base;
    key->getkey = getkey;
    key->callback = callback;
    key->trig = trig;
    key->dstart = 0;
    key->state = QKEY_NO_ACTION;
    key->dtime = dtime;
    key->dcnt = dtime;
    list_insert_before(&key->qkey_node, &key_list);
    key_id_base ++;
}

void qkey_exec()
{
    ListObj* node;
    QKeyObj* key;
    int state;

    list_for_each(node, &key_list){
        key = list_entry(node, QKeyObj, qkey_node);
        if(key->getkey() == key->trig){
            key->dstart = 1;
        }
        if(key->state == QKEY_IS_PRESSED){
            key->callback();
            key->state = QKEY_NO_ACTION;
            key->dstart = 0;
        }
    }
}

void qkey_tick()
{
    ListObj* node;
    QKeyObj* key;
    
    list_for_each(node, &key_list){
        key = list_entry(node, QKeyObj, qkey_node);
        if(key->dstart == 1){
            key->dcnt --;
        }
        if(key->dcnt == 0){
            key->state = QKEY_IS_PRESSED;
            key->dcnt = key->dtime;
        }
    }
}

