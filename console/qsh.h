/*
 * @Author: luoqi 
 * @Date: 2021-05-26 16:10:33 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-08-31 16:51:49
 */

#ifndef _QSH_H
#define _QSH_H

#include <stdio.h>
#include "../kernel/cmd.h"

#define QSH_HISTORY_MAX     10

typedef enum
{
    QSH_RECV_SPEC,
    QSH_RECV_UNFINISH,
    QSH_RECV_NOCMD,
    QSH_RECV_FINISHED
} QshRecvState;

void qsh_get_cmd(char recv_byte);
void qsh_init(void);
void qsh_task_exec(void);
void qsh_input_logo(void);

#define QSH_PRINTF(...) printf(__VA_ARGS__)

typedef CmdObj     QshCmd;

void qsh_cmd_creat(QshCmd* qcmd,
                    const char* name,
                    unsigned char param_num,
                    unsigned char (*handle)(int, char**),
                    const char* usage);

#endif
