/*
 * @Author: luoqi 
 * @Date: 2021-05-26 16:10:33 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-05-26 16:12:23
 */

#ifndef _CONSOLE_H
#define _CONSOLE_H

typedef enum 
{
    DBG_NO_ERR = 0,
} DbgErrType;

typedef enum
{
    UNFINISH,
    NOCMD,
    FINISHED
} CmdRecvState;

void dbg_cmd_console(char data);
DbgErrType dbg_task_init(void);
DbgErrType dbg_task_exec(void);
void dbg_set_cmd_recv_state(CmdRecvState state);
void dbg_cmd_reset(void);
CmdRecvState dbg_get_cmd_recv_state(void);

#endif
