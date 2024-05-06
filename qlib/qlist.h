/*
 * @Author: luoqi 
 * @Date: 2021-05-07 21:18:06 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 16:18:53
 */

#ifndef _QLIST_H
#define _QLIST_H

#ifdef __cplusplus
 extern "C" {
#endif

#define QLIST_ENTRY(node, type, member)            ((type *)((char *)(node) - ((unsigned long) &((type*)0)->member)))

#define QLIST_ITERATOR(node, list)               for(node = (list)->next; node != (list); node = node->next)            

#define QLIST_ITERATER_SAFE(node, cache, list)   for(node = (list)->next, cache = node->next; node != (list); node = cache, cache = node->next)

#define QLIST_CREATE(name)                        QList name = {&name, &name}        

typedef struct _qlist
{
    struct _qlist* next;
    struct _qlist* prev;
} QList;

void qlist_init(QList* list);

void qlist_insert_after(QList* list, QList* node);

void qlist_insert_before(QList* list, QList* node);

void qlist_remove(QList* node);

int qlist_isempty(const QList* list);

int qlist_isexist(const QList* list, const QList* node);

unsigned int qlist_length(const QList* list);

#ifdef __cplusplus
 }
#endif

#endif
