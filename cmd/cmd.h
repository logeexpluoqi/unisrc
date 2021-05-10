/*
 * @Author: luoqi 
 * @Date: 2021-04-29 00:30:20 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-04-29 19:53:28
 */

#ifndef _CMD_H
#define _CMD_H

#include "../common/list.h"
typedef struct cmd_object
{
    char* name;
    unsigned int id;
    unsigned char (*cmd_hdl)(int, const char* []);
    char* usage; 
    ListObj cmd_list;
} CmdObj;

unsigned char cmd_exec(CmdObj* obj, int argc, const char* argv[]);
void cmd_init(CmdObj* obj, 
              char* name,
              unsigned int id,
              unsigned char(*cmd_hdl)(int, const char* []), 
              char* usage
             );
void cmd_add(CmdObj* obj);
unsigned char cmd_del(CmdObj* obj);
unsigned char cmd_isexist(CmdObj* obj);
unsigned int cmd_num(void);

#endif
