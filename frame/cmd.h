/*
 * @Author: luoqi 
 * @Date: 2021-04-29 00:30:20 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 19:18:48
 */

#ifndef _CMD_H
#define _CMD_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "qlist.h"
#define CMD_MAX_LEN     60
#define CMD_MAX_NUM     10

typedef enum cmd_err
{
    CMD_PARAM_ERR       = -1,
    CMD_NO_ERR          = 0,
    CMD_LEN_OUT         = 1,
    CMD_NUM_OUT         = 2,
    CMD_NO_CMD          = 3,
    CMD_PARAM_EXCEED    = 4,
    CMD_PARAM_LESS      = 5,
    CMD_EXEC_ERR        = 6
} CmdErrType;

typedef struct cmd_object
{
    const char* name;
    /* id auto increase from 1, if return id is 0 means that there is no target command */
    unsigned int id;
    /* parameter number, max 255 */
    unsigned char param_num;
    int (*cmd_hdl)(int, char* []);
    const char* usage;
    ListObj cmd_list;
} CmdObj;

CmdErrType cmd_exec(char* cmd_msg);

void cmd_init(CmdObj* cmd,
              const char* name,
              unsigned char param_num, // if this value is 0xff, means that no arg number limit
              int(*cmd_hdl)(int, char* []),
              const char* usage);

void cmd_add(CmdObj* cmd);

void cmd_del(CmdObj* cmd);

unsigned char cmd_isexist(CmdObj* cmd);

unsigned int cmd_get_id(CmdObj* cmd);

unsigned int cmd_num(void);

CmdObj* cmd_obj_get(unsigned int cmd_id);

#ifdef __cplusplus
 }
#endif

#endif
