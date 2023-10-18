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

#include <stdint.h>

#define CLIST_OBJ(ptr, type, member)     ((type *)((char *)(ptr) - ((unsigned long) &((type*)0)->member)))

#define CLIST_ITERATOR(node, clist)      for (node = (clist)->next; node != (clist); node = node->next)

#define CMD_MAX_LEN     60
#define CMD_MAX_NUM     10

    typedef enum cmd_err
    {
        CMD_EXEC_ERR        = -2,
        CMD_PARAM_ERR       = -1,
        CMD_EOK             = 0,
        CMD_LENGTH_OUT      = 1,
        CMD_MISSING         = 2,
        CMD_PARAM_OVERFLOW  = 3,
        CMD_PARAM_LESS      = 4,
    } CmdInfo;

    typedef struct _clist {
        struct _clist *prev;
        struct _clist *next;
    } CList;

    typedef int (*CmdCallback)(int, char **);
    typedef struct
    {
        const char *name;
        uint32_t    id;
        CmdCallback callback;
        const char *usage;
        CList       node;
    } CmdObj;

    CmdInfo cmd_exec(char *cmd_msg);

    int cmd_init(CmdObj *cmd, const char *name, CmdCallback callback, const char *usage);

    int cmd_add(CmdObj *cmd);

    int cmd_del(CmdObj *cmd);

    int cmd_isexist(CmdObj *cmd);

    uint32_t cmd_num(void);

    CmdObj *cmd_obj(uint32_t id);

#ifdef __cplusplus
}
#endif

#endif
