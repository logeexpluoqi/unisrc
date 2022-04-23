/*
 * @Author: luoqi 
 * @Date: 2021-05-11 10:02:21 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 18:41:34
 */

#include "msg.h"

#define NULL (void*) 0

static unsigned char msg_strcmp(const char* s1, const char* s2);
static unsigned int msg_container_num = 0;

void msg_container_init(MsgContainer* container, const char* name, const char* usage)
{
    container->name = name;
    container->usage = usage;
    list_init(&container->msg_list);
    msg_container_num++;
}

void msg_init(MsgObj* obj,
    const char* name,
    void* mem,
    MsgType type,
    const char* usage)
{
    obj->name = name;
    obj->mem = mem;
    obj->type = type;
    obj->is_update = 0;
    obj->msg_state = MSG_UNLOCK;
    obj->usage = usage;
}

void msg_add(MsgContainer* container, MsgObj* obj)
{
    list_insert_before(&container->msg_list, &obj->msg_list_node);
}

void msg_del(MsgObj* obj)
{
    list_remove(&obj->msg_list_node);
}

void* msg_get(MsgContainer* container, const char* name)
{
    ListObj* node;
    MsgObj* msg;

    list_for_each(node, &container->msg_list) {
        msg = list_entry(node, MsgObj, msg_list_node);
        if (msg_strcmp(name, msg->name) == 0 && msg->msg_state == MSG_UNLOCK)
        {
            msg->is_update = 0;
            return msg->mem;
        }
    }
    return NULL;
}

unsigned char msg_set(MsgContainer* container, const char* name, void* msg_data, unsigned int size)
{
    unsigned int i;
    ListObj* node;
    MsgObj* msg;

    list_for_each(node, &container->msg_list) {
        msg = list_entry(node, MsgObj, msg_list_node);
        msg->msg_state = MSG_LOCKED;
        if (msg_strcmp(name, msg->name) == 0) {
            switch (msg->type) {
            case MSG_CHAR: 
                for(i = 0; i < size; i ++)
                    ((char*)msg->mem)[i] = ((char*)msg_data)[i];
                break;
            case MSG_UCHAR: 
                for(i = 0; i < size; i ++)
                    ((unsigned char*)msg->mem)[i] = ((unsigned char*)msg_data)[i];
                break;
            case MSG_INT: 
                for(i = 0; i < size; i ++)
                    ((int*)msg->mem)[i] = ((int*)msg_data)[i];
                break;
            case MSG_UINT: 
                for(i = 0; i < size; i ++)
                    ((unsigned int*)msg->mem)[i] = ((unsigned int*)msg_data)[i];
                break;
            case MSG_FLOAT: 
                for(i = 0; i < size; i ++)
                    ((float*)msg->mem)[i] = ((float*)msg_data)[i];
                break;
            case MSG_DOUBLE: 
                for(i = 0; i < size; i ++)
                    ((double*)msg->mem)[i] = ((double*)msg_data)[i];
                break;
            case MSG_LINT: 
                for(i = 0; i < size; i ++)
                    ((long int*)msg->mem)[i] = ((long int*)msg_data)[i];
                break;
            case MSG_LLINT: 
                for(i = 0; i < size; i ++)
                    ((long long int*)msg->mem)[i] = ((long long int*)msg_data)[i];
                break;
            }
            msg->msg_state = MSG_UNLOCK;
            msg->is_update = 1;
        }
    }
    return 0;
}

unsigned char msg_is_update(MsgContainer* container, const char* name)
{
    ListObj* node;
    MsgObj* msg;
    list_for_each(node, &container->msg_list) {
        msg = list_entry(node, MsgObj, msg_list_node);
        if(msg_strcmp(msg->name, name) == 0)
           return msg->is_update; 
    }
    return 0;
}

unsigned char msg_mem_location_set(MsgContainer* container, const char* name, void* new_mem)
{
    ListObj* node;
    MsgObj* msg;

    list_for_each(node, &container->msg_list) {
        msg = list_entry(node, MsgObj, msg_list_node);
        msg->msg_state = MSG_LOCKED;
        if (msg_strcmp(name, msg->name) == 0) {
            msg->mem = new_mem;
            msg->msg_state = MSG_UNLOCK;
            msg->is_update = 1;
        }
    }
    return 0;
}

unsigned int msg_container_num_get()
{
    return msg_container_num;
}

unsigned int msg_num_get(MsgContainer* container)
{
    return list_len(&container->msg_list);
}

MsgObj* msg_obj_get(MsgContainer* container, unsigned int serial)
{
    ListObj* node = &container->msg_list;
    for (unsigned int i = serial; i > 0; i--) {
        node = node->next;
    }
    return list_entry(node, MsgObj, msg_list_node);
}

static unsigned char msg_strcmp(const char* s1, const char* s2)
{
    unsigned int i = 0;

    while (s1[i] != 0 || s2[i] != 0) {
        if (((s1[i] == 0) && (s2[i] != 0)) || ((s1[i] != 0) && (s2[i] == 0)) || (s1[i] != s2[i])) {
            return 1;
        }
        i++;
    }
    return 0;
}
