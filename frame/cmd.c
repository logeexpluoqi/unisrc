/*
 * @Author: luoqi 
 * @Date: 2021-04-29 00:29:54 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-06-13 22:12:19
 */

#include "cmd.h"

#define ASCII_SPACE 0x20
static LIST_HEAD(cmd_list);

static unsigned int cmd_id = 1;

static unsigned char cmd_strcmp(const char* s1, const char* s2);

CmdErrType cmd_exec(char* args)
{
    ListObj* node;
    CmdObj* cmd;
    unsigned int argc;
    char* argv[CMD_MAX_NUM + 1];
    unsigned int argn = 0;
    unsigned char i = 0;
    
    /* cmd parser */
    do{
        if (i < 1) {
            argv[argn] = &args[i];
        }
        else if(args[i] == ASCII_SPACE) {
            argv[++argn] = &args[i + 1];
            args[i] = 0;
        }
        i++;

        if (i > CMD_MAX_LEN - 1) {
            return CMD_LEN_OUT;
        }

        if (argn > CMD_MAX_NUM - 1) {
            return CMD_NUM_OUT;
        }
        
    } while(args[i] != 0);

    argc = argn + 1; // add 1 because there is no space key value front of the first argv
    /* End of dismantling input command string */

    list_for_each(node, &cmd_list){
        cmd = list_entry(node, CmdObj, cmd_list);
        if (cmd_strcmp(cmd->name, argv[0]) == 0) {
            if(cmd->param_num != 0xff) {
                if (cmd->param_num > argc - 1) {
                    return CMD_PARAM_LESS;
                } else if (cmd->param_num < argc - 1) {
                    return CMD_PARAM_EXCEED;
                }

                if (cmd->cmd_hdl(argc, argv) < 0) {
                    return CMD_EXEC_ERR;
                } else {
                    return CMD_NO_ERR;
                }
            } else {
                if(cmd->cmd_hdl(argc, argv) < 0) {
                    return CMD_EXEC_ERR;
                } else {
                    return CMD_NO_ERR;
                }
            }
        }
    }
    return CMD_NO_CMD;
}

void cmd_init(CmdObj* cmd, const char* name, unsigned char param_num, int (*cmd_hdl)(int, char* []), const char* usage)
{
    cmd->name = name;
    cmd->param_num = param_num;
    cmd->id = cmd_id;
    cmd->cmd_hdl = cmd_hdl;
    cmd->usage = usage;
    cmd_id++;
}

void cmd_add(CmdObj* cmd)
{
    if(cmd_isexist(cmd) == 0) {
        list_insert_before(&cmd_list, &cmd->cmd_list);
    } else {
        return;
    }
}

void cmd_del(CmdObj* cmd)
{
    if(cmd_isexist(cmd)) {
        list_remove(&cmd->cmd_list);
    } else {
        return;
    }
}

unsigned char cmd_isexist(CmdObj* cmd)
{
    unsigned char isexist = 0;
    ListObj* node;
    CmdObj* _cmd;

    list_for_each(node, &cmd_list) {
        _cmd = list_entry(node, CmdObj, cmd_list);
        if(cmd->id == _cmd->id) {
            isexist = 1;
        } else {
            continue;
        }
    }

    return isexist;
}

unsigned int cmd_get_id(CmdObj* cmd)
{
    return cmd->id;
}

unsigned int cmd_num()
{
    return list_len(&cmd_list);
}

unsigned char cmd_strcmp(const char* s1, const char* s2)
{
    unsigned int i = 0;

    while (s1[i] != 0 || s2[i] != 0) {
        if (s1[i] != s2[i]) {
            return 1;
        } else {
            i++;
        }
    }

    return 0;
}

CmdObj* cmd_obj_get(unsigned int cmd_id)
{
    ListObj* node = &cmd_list;

    for(unsigned int i = cmd_id; i > 0; i--) {
        node = node->next;
    }

    return list_entry(node, CmdObj, cmd_list);
}
