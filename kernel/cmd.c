/*
 * @Author: luoqi 
 * @Date: 2021-04-29 00:29:54 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-09-14 18:17:25
 */

#include "cmd.h"

#define ASCII_SPACE 0x20
static LIST_HEAD(cmd_list);

static unsigned int cmd_id = 1;

static unsigned char cmd_strcmp(const char* s1, const char* s2);

CmdErrType cmd_exec(char* cmd_msg)
{
    ListObj* node;
    CmdObj* cmd;
    unsigned int argc;
    char* argv[CMD_MAX_NUM + 1];

    /* This section is used to dismantling input command string */
    unsigned char i = 0;
    unsigned int argn = 0;

    while (cmd_msg[i] != 0)
    {
        if (i > CMD_MAX_LEN - 1) {
            return CMD_LEN_OUT;
        }
        else if (argn > CMD_MAX_NUM - 1) {
            return CMD_NUM_OUT;
        }

        if (i == 0)
        {
            argv[argn] = &cmd_msg[i];
        }
        if (cmd_msg[i] == ASCII_SPACE)
        {
            argv[++argn] = &cmd_msg[i + 1];
            cmd_msg[i] = 0;
        }
        i++;
    }
    argc = argn + 1; // add 1 because there is no space key value front of the first argv
    /* End of dismantling input command string */

    list_for_each(node, &cmd_list)
    {
        cmd = list_entry(node, CmdObj, cmd_list);
        if (cmd_strcmp(cmd->name, argv[0]) == 0)
        {
            if(cmd->param_num != 0xff)
            {
                if (cmd->param_num > argc - 1)
                {
                    return CMD_PARAM_LESS;
                }
                else if (cmd->param_num < argc - 1)
                {
                    return CMD_PARAM_EXCEED;
                }

                if (cmd->cmd_hdl(argc, argv) != 0)
                {
                    return CMD_EXEC_ERR;
                }
                else
                {
                    return CMD_NO_ERR;
                }
            }
            else
            {
                if(cmd->cmd_hdl(argc, argv) != 0)
                    return CMD_EXEC_ERR;
                else
                    return CMD_NO_ERR;
            }
        }
    }

    return CMD_NO_CMD;
}

void cmd_init(CmdObj* obj,
              const char* name,
              unsigned char param_num,
              unsigned char(*cmd_hdl)(int, char* []),
              const char* usage)
{
    obj->name = name;
    obj->param_num = param_num;
    obj->id = cmd_id;
    obj->cmd_hdl = cmd_hdl;
    obj->usage = usage;
    cmd_id++;
}

void cmd_add(CmdObj* obj)
{
    if(cmd_isexist(obj) == 0)
    {
        list_insert_before(&cmd_list, &obj->cmd_list);
    }
}

void cmd_del(CmdObj* obj)
{
    if(cmd_isexist(obj))
    {
        list_remove(&obj->cmd_list);
    }
}

unsigned char cmd_isexist(CmdObj* obj)
{
    unsigned char isexist = 0;
    ListObj* node;
    CmdObj* cmd;

    list_for_each(node, &cmd_list)
    {
        cmd = list_entry(node, CmdObj, cmd_list);
        if (obj->id == cmd->id)
        {
            isexist = 1;
        }
    }

    return isexist;
}

unsigned int cmd_get_id(CmdObj* obj)
{
    return obj->id;
}

unsigned int cmd_num()
{
    return list_len(&cmd_list);
}

unsigned char cmd_strcmp(const char* s1, const char* s2)
{
    unsigned int i = 0;

    while (s1[i] != 0 || s2[i] != 0)
    {
        if (((s1[i] == 0) && (s2[i] != 0)) || ((s1[i] != 0) && (s2[i] == 0)) || (s1[i] != s2[i]))
        {
            return 1;
        }
        i++;
    }

    return 0;
}

CmdObj* cmd_obj_get(unsigned int serial)
{
    ListObj* node = &cmd_list;

    for(unsigned int i = serial; i > 0; i--)
        node = node->next;

    return list_entry(node, CmdObj, cmd_list);
}
