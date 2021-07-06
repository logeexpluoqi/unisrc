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

void console_cmd_recv(char data);
DbgErrType console_task_init(void);
DbgErrType console_task_exec(void);
void console_print_usr_head(void);

extern kprintf(char* fmt, ...);

#endif
