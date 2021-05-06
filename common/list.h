/*
 * @Author: luoqi 
 * @Date: 2021-04-29 13:51:33 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-04-29 16:51:34
 */

#ifndef _LIST_H
#define _LIST_H


typedef struct list_structure
{
    struct list_structure* next;
    struct list_structure* prev;
} ListObj;

#define LIST_CREAT(name) ListObj name = {&(name), &(name)}
#define container_of(ptr, type, member)     \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

void list_init(ListObj* list);
void list_insert_after(ListObj* list, ListObj* node);
void list_insert_before(ListObj* list, ListObj* node);
void list_remove(ListObj* node);
int list_isempty(const ListObj* list);
unsigned int list_length(const ListObj* list);

#define list_entry(node, type, member) \
    container_of(node, type, member)

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

#define list_for_each_entry(pos, head, member) \
    for (pos = rt_list_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = rt_list_entry(pos->member.next, typeof(*pos), member))

#define list_for_each_entry_safe(pos, n, head, member) \
    for (pos = rt_list_entry((head)->next, typeof(*pos), member), \
         n = rt_list_entry(pos->member.next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = n, n = rt_list_entry(n->member.next, typeof(*n), member))

#endif


