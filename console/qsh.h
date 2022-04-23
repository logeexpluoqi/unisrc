/*
 * @Author: luoqi 
 * @Date: 2021-05-26 16:10:33 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 17:31:09
 */

#ifndef _QSH_H
#define _QSH_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include "../kernel/cmd.h"

#define QSH_HISTORY_MAX     10

#define QSH(...)        printf(__VA_ARGS__)

typedef CmdObj  QshCmd;

typedef enum
{
    QSH_RECV_SPEC,
    QSH_RECV_UNFINISH,
    QSH_RECV_NOCMD,
    QSH_RECV_FINISHED
} QshRecvState;

void qsh_get_char(char recv_byte);

void qsh_init(void);

void qsh_task_exec(void);

void qsh_cmd_init(QshCmd *qcmd,
                  const char *name,
                  unsigned char (*handle)(int, char**),
                  const char* usage);

void qsh_cmd_add(QshCmd* qcmd);

void qsh_cmd_del(QshCmd* qcmd);

#ifdef __cplusplus
 }
#endif

#endif
