/*
 * @Author: luoqi 
 * @Date: 2021-05-26 16:10:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-05-26 16:15:03
 */

#include "console.h"
#include "../cmd/cmd.h"
#include "../timeslice/timeslice.h"
#include <string.h>

static char cmd_buf[CMD_MAX_LEN];
// static char cmd_hs_buf[10][CMD_MAX_LEN]; 
static CmdRecvState cmd_recv_state = NOCMD;
static unsigned int recv_cnt = 0;

DbgErrType dbg_task_init()
{

    return DBG_NO_ERR;
}

void dbg_cmd_console(char recv_byte)
{
    // static char hs_pos = 0;
    /* \r: return, 38: up, 40: down, \b: backspace */
    if(recv_byte != '\r' && recv_byte != 38 && \
       recv_byte != 40   && recv_byte != '\b' && \
       recv_cnt  <  60)
    { // normal charactor
        cmd_buf[recv_cnt++] = recv_byte;
        printf("%c", recv_byte);
    }
    else if(recv_byte == '\b' && recv_cnt > 0)
    { // backspace charactor
        for(char i = 0; i < recv_cnt; i++)
        {
            printf(" "); 
        }
        cmd_buf[recv_cnt--] = 0;
        printf("\r$ ");
        for(char i = 0; i < recv_cnt; i++)
        {
            printf("%c", cmd_buf[i]);
        }
    }
    else if(recv_byte == '\r')
    { // return charactor
        if(recv_cnt != 0)
        {
            dbg_set_cmd_recv_state(FINISHED);
            // hs_pos = cmd_hs_add();
            printf("\r\n");
            recv_cnt = 0;
        }
        else 
            printf("\r\n$ ");
    }
    else if(recv_byte == 38) // history prev
    { // up charactor
        // printf("\r$ asdffdsadfsa%s\r\n", cmd_hs_get(hs_pos));
        // memcpy(cmd_buf, cmd_hs_buf[hs_pos], sizeof(cmd_buf));
        
        // hs_pos --;
    }
    else if(recv_byte == 40) // history next
    { // down charactor
        
    }
    else 
    {
        printf("\r\n>> Err: Command buffer overflow !\r\n$ ");
        dbg_cmd_reset();
        recv_cnt = 0;
    }
}

void dbg_cmd_reset()
{
    recv_cnt = 0;
    memset(cmd_buf, 0, sizeof(cmd_buf));
}

char* dbg_cmd()
{
    if(cmd_recv_state == FINISHED)
    {
        // printf("# Finished: %s\r\n", cmd_buf);
        return cmd_buf;
    }
    else 
    {
        // printf("# NULL: %s\r\n", cmd_buf);
        return NULL;
    }
}

void dbg_set_cmd_recv_state(CmdRecvState state)
{
    cmd_recv_state = state;
}

CmdRecvState dbg_get_cmd_recv_state()
{
    return cmd_recv_state;
}

DbgErrType dbg_task_exec()
{
    if(cmd_recv_state == FINISHED)
    {
        switch(cmd_exec(dbg_cmd()))
        {
        case CMD_NO_ERR: break;
        case CMD_LEN_OUT: 
        {
            printf(">> Err: CMD_LEN_OUT\r\n");
            break;
        }
        case CMD_NUM_OUT:
        {
            printf(">> Err: CMD_NUM_OUT\r\n");
            break;
        }
        case CMD_NO_CMD:
        {
            printf(">> Err: CMD_NO_CMD\r\n");
            break;
        }
        case CMD_PARAM_EXCEED:
        {
            printf(">> Err: CMD_PARAM_EXCEED\r\n");
            break;
        }
        case CMD_PARAM_LESS: 
        {
            printf(">> Err: CMD_PARAM_LESS\r\n");
            break;
        }
        case CMD_EXEC_ERR:
        {
            printf(">> Err: CMD_EXEC_ERR\r\n");
            break;
        }
        default:
            break;
        }
        recv_cnt = 0;
        printf("$ ");
        memset(cmd_buf, 0, sizeof(cmd_buf));
        dbg_cmd_reset();
        cmd_recv_state = NOCMD;
    }
    return DBG_NO_ERR;
}

unsigned char dbg_help_hdl(int argc, char* argv[])
{
    CmdObj* cmd;

    unsigned int num = cmd_num(); // except cmd_list head
    printf(" CMD NUM: %d \r\n", num);
    printf(" [Commands]           [Usage]\r\n");
    for(unsigned int i = num; i > 0; i--)
    {
        cmd = cmd_obj_get(i);
        printf("  %-15s :    %s\r\n", cmd->name, cmd->usage);
    }
    printf("\r\n");
    return 0;
}

unsigned char dbg_ls_hdl(int argc, char* argv[])
{
    if(strcmp(argv[1],"task") == 0)
    {
        TimesilceTaskObj* task;
        unsigned int num = timeslice_get_task_num();
        printf(" TASK NUM: %d, TIME TICK: %dus \r\n", num, 100);
        printf(" [Task name]                [Timeslice]            [Usage]\r\n");
        for(unsigned int i = num; i > 0; i--)
        {
            task = timeslice_obj_get(i);
            printf("  %-20s :     %-6d          :      %s\r\n", task->name, task->timeslice_len, task->usage);
        }
    }
    else 
    {
        printf(">> Err: parameter not find !");
    }
    printf("\r\n");
    return 0;
}
