/*
 * @Author: luoqi 
 * @Date: 2021-04-29 00:29:54 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-04-29 19:54:34
 */

#include "./cmd.h"

static LIST_HEAD(cmd_list);

unsigned char cmd_exec(CmdObj* obj, int argc, const char* argv[])
{
    ListObj* node;
    CmdObj* cmd;
    unsigned char state = 0;

    list_for_each(node, &cmd_list)
    {
        cmd = list_entry(node, CmdObj, cmd_list);
        if(obj->id == cmd->id)
        {
            state = cmd->cmd_hdl(argc, argv);
        }
    }
    return state;
}

void cmd_init(CmdObj* obj, 
              char* name,
              unsigned int id,
              unsigned char(*cmd_hdl)(int, const char* []), 
              char* usage)
{
    obj->name = name;
    obj->id = id;
    obj->cmd_hdl = cmd_hdl;
    obj->usage = usage;
}

void cmd_add(CmdObj* obj)
{
    list_insert_before(&cmd_list, &obj->cmd_list);
}

unsigned char cmd_del(CmdObj* obj)
{
    if(cmd_isexist(obj))
    {
        list_remove(&obj->cmd_list);
        return 0;
    }
    else 
        return 1;
}
 
unsigned char cmd_isexist(CmdObj* obj)
{
    unsigned char isexist = 0;
    ListObj* node;
    CmdObj* cmd;

    list_for_each(node, &cmd_list)
    {
        cmd = list_entry(node, CmdObj, cmd_list);
        if(obj->id == cmd->id)
        {
            isexist = 1;
        }
    }

    return isexist;
}

unsigned int cmd_num()
{
    return list_len(&cmd_list);
}

