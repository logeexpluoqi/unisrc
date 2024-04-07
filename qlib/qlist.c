/*
 * @Author: luoqi
 * @Date: 2021-04-29 13:51:58
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 18:41:39
 */

#include "qlist.h"

void qlist_init(QList *list)
{
    list->next = list->prev = list;
}

void qlist_insert_after(QList *list, QList *node)
{
    list->next->prev = node;
    node->next = list->next;

    list->next = node;
    node->prev = list;
}

void qlist_insert_before(QList *list, QList *node)
{
    list->prev->next = node;
    node->prev = list->prev;

    list->prev = node;
    node->next = list;
}

void qlist_remove(QList *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;

    node->next = node->prev = node;
}

int qlist_isempty(const QList *list)
{
    return list->next == list;
}

int qlist_isexist(const QList* list, const QList* node)
{
    QList *_node;
    QLIST_ITERATOR(_node, list) {
        if(_node == node) {
            return 1;
        } else {
            continue;
        }
    }
    return 0;
}

unsigned int qlist_len(const QList *list)
{
    unsigned int len = 0;
    const QList *p = list;
    while(p->next != list) {
        p = p->next;
        len++;
    }

    return len;
}
