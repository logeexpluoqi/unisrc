/*
 * @Author: luoqi 
 * @Date: 2021-05-07 21:18:06 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 16:18:53
 */

#ifndef _QLIST_H
#define _QLIST_H

#define offset_of(type, member)             ((unsigned long) &((type*)0)->member)
#define container_of(ptr, type, member)     ((type *)((char *)(ptr) - offset_of(type, member)))

typedef struct list_structure
{
    struct list_structure* next;
    struct list_structure* prev;
} ListObj;

#define LIST_HEAD_INIT(name)    {&(name), &(name)}
#define LIST_HEAD(name)         ListObj name = LIST_HEAD_INIT(name)

void list_init(ListObj* list);
void list_insert_after(ListObj* list, ListObj* node);
void list_insert_before(ListObj* list, ListObj* node);
void list_remove(ListObj* node);
int list_isempty(const ListObj* list);
unsigned int list_len(const ListObj* list);

#define list_entry(node, type, member) \
    container_of(node, type, member)

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

#endif
