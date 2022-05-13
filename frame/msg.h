/*
 * @Author: luoqi 
 * @Date: 2021-05-11 10:02:35 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 16:20:10
 */
#ifndef _MSG_H
#define _MSG_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "qlist.h"

typedef enum MSG_STATE
{
    MSG_LOCKED,
    MSG_UNLOCK
} MsgState;

typedef enum MSG_TYPE
{
    MSG_CHAR,
    MSG_UCHAR,
    MSG_INT,
    MSG_UINT,
    MSG_FLOAT,
    MSG_DOUBLE,
    MSG_LINT,
    MSG_LLINT
} MsgType;

typedef struct msg_container
{
    const char* name;
    const char* usage;
    ListObj msg_list;
} MsgContainer;

typedef struct msg_obj
{
    const char* name;
    void* mem;
    MsgState msg_state;
    MsgType type;
    unsigned char is_update;
    const char* usage;
    ListObj msg_list_node;
} MsgObj;

#define MSG_CREAT(name, mem, type, size)     MsgObj name; type mem[size]
#define MSG_SIZE(mem, type)                 (sizeof(mem)) / (sizeof(type))

void msg_container_init(MsgContainer* container,
                        const char* name,
                        const char* usage);

void msg_init(MsgObj* obj,
              const char* name,
              void* mem,
              MsgType type,
              const char* usage);

void msg_add(MsgContainer* container, MsgObj* obj);

void msg_del(MsgObj* obj);

void* msg_get(MsgContainer* container, const char* name);

unsigned char msg_set(MsgContainer* container, const char* name, void* msg_data, unsigned int size);

unsigned char msg_is_update(MsgContainer* container, const char* name);

unsigned char msg_mem_location_set(MsgContainer* container, const char* name, void* new_mem);

unsigned int msg_container_num_get(void);

unsigned int msg_num_get(MsgContainer* container);

MsgObj* msg_obj_get(MsgContainer* container, unsigned int serial);

#ifdef __cplusplus
 }
#endif

#endif
