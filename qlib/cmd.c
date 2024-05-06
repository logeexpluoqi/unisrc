/*
 * @Author: luoqi
 * @Date: 2021-04-29 00:29:54
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-06-13 22:12:19
 */

#include "cmd.h"

#define ASCII_SPACE     0x20
#define ASCII_DOT       0x2c

static CList clist = { &clist, &clist };
static uint32_t id = 1;

static int _strcmp(const char *s1, const char *s2)
{
    uint32_t i = 0;

    while(s1[i] != 0 || s2[i] != 0) {
        if(s1[i] != s2[i]) {
            return 1;
        } else {
            i++;
        }
    }

    return 0;
}

static inline void clist_insert(CList *list, CList *node)
{
    list->next->prev = node;
    node->next = list->next;

    list->next = node;
    node->prev = list;
}

static inline void cqlist_remove(CList *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;

    node->next = node->prev = node;
}

CmdInfo cmd_exec(char *args)
{
    CList *_node;
    CmdObj *cmd;
    char *argv[CMD_MAX_NUM + 1] = { 0 };
    int argc = 0;
    int argn = 0;
    int i = 0;

    /* cmd parser */
    do {
        if(i == 0) {
            argv[argn] = args + i;
        } else if(args[i] == ASCII_SPACE || args[i] == ASCII_DOT) {
            args[i] = 0;
            if(args[i + 1] != ASCII_SPACE && args[i + 1] != ASCII_DOT) {
                argv[++argn] = args + i + 1;
            }
        }
        i++;
        if(i > CMD_MAX_LEN - 1) {
            return CMD_LENGTH_OUT;
        }

        if(argn > CMD_MAX_NUM - 1) {
            return CMD_PARAM_OVERFLOW;
        }

    } while(args[i] != 0);
    /* End of dismantling input command string */
    /* add 1 because there is no space key value front of the first argv */
    argc = argn + 1;
    CLIST_ITERATOR(_node, &clist) {
        cmd = CLIST_ENTRY(_node, CmdObj, node);
        if(_strcmp(cmd->name, argv[0]) == 0) {
            return cmd->callback(argc, argv);
        }
    }
    return CMD_MISSING;
}

int cmd_init(CmdObj *cmd, const char *name, CmdCallback callback, const char *usage)
{
    cmd->name = name;
    cmd->id = id;
    cmd->callback = callback;
    cmd->usage = usage;
    id++;
    return 0;
}

int cmd_add(CmdObj *cmd)
{
    if(cmd_isexist(cmd) == 0) {
        clist_insert(&clist, &cmd->node);
        return 0;
    } else {
        return -1;
    }
}

int cmd_del(CmdObj *cmd)
{
    if(cmd_isexist(cmd)) {
        cqlist_remove(&cmd->node);
        return 0;
    } else {
        return -1;
    }
}

int cmd_isexist(CmdObj *cmd)
{
    CList *_node;
    CmdObj *_cmd;

    CLIST_ITERATOR(_node, &clist) {
        _cmd = CLIST_ENTRY(_node, CmdObj, node);
        if(cmd->id == _cmd->id) {
            return 1;
        } else {
            continue;
        }
    }
    return 0;
}

uint32_t cmd_num()
{
    unsigned int len = 0;
    const CList *l = &clist;
    while (l->next != &clist) {
        l = l->next;
        len++;
    }

    return len;
}

CmdObj *cmd_obj(uint32_t id)
{
    CList *_node = &clist;

    for(uint32_t i = id; i > 0; i--) {
        _node = _node->next;
    }

    return CLIST_ENTRY(_node, CmdObj, node);
}
