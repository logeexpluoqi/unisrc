/*
 * @Author: luoqi 
 * @Date: 2021-05-26 16:10:33 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-05-26 16:12:23
 */

#ifndef _QSH_H
#define _QSH_H

typedef enum
{
    QSH_RECV_UNFINISH,
    QSH_RECV_NOCMD,
    QSH_RECV_FINISHED
} QshRecvState;

void qsh_get_cmd(char recv_byte);
void qsh_task_init(void);
void qsh_task_exec(void);
void qsh_input_logo(void);

extern qsh_printf(char* fmt, ...);

#endif
