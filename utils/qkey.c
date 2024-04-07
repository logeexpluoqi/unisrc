/*
 * @Author: luoqi
 * @Date: 2021-10-17 20:29:15
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-10 23:32:47
 */

#include "qkey.h"

static QLIST_CREATE(key_list);
static unsigned int key_id_base = 0;

int qkey_init(QKeyObj *key,
              const char *name,
              QKeyState state,
              QKeyState(*getkey)(void),
              int (*callback)(void),
              unsigned int dtime)
{
    key->name = name;
    key->id = key_id_base;
    key->getkey = getkey;
    key->callback = callback;
    key->state = state;
    key->dstart = 0;
    key->dtime = dtime;
    key->dcnt = dtime;
    qlist_insert_before(&key->qkey_node, &key_list);
    key_id_base++;
    return 0;
}

int qkey_exec()
{
    QList *node, *_node;
    QKeyObj *key;
    int state;

    QLIST_ITERATER_SAFE(node, _node, &key_list) {
        key = QLIST_OBJ(node, QKeyObj, qkey_node);
        if (key->getkey() == key->state) {
            key->dstart = 1;
        }
    }
    return 0;
}

int qkey_tick()
{
    QList *node, *_node;
    QKeyObj *key;

    QLIST_ITERATER_SAFE(node, _node, &key_list) {
        key = QLIST_OBJ(node, QKeyObj, qkey_node);
        if (key->dstart == 1) {
            key->dcnt--;
        }
        if (key->dcnt == 0) {
            if (key->state == key->getkey()) {
                key->err = key->callback();
            }
            key->dcnt = key->dtime;
        }
    }
    return 0;
}
