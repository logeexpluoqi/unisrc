/*
 * @Author: luoqi 
 * @Date: 2021-05-26 16:10:33 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-08-13 23:18:44
 */

#ifndef _QSH_H
#define _QSH_H

#ifdef __cplusplus
 extern "C" {
#endif

#define QSH_USING_LIBC    1

#ifdef QSH_USING_LIBC
#include <stdlib.h>
#endif
#include <stdio.h>
#include <string.h>
#include "../frame/cmd.h"

#define QSH_HISTORY_MAX             10

#define QSH(...)                    printf(__VA_ARGS__)

#define QSH_ISARG(str1, str2)       (strcmp(str1, str2) == 0)

typedef CmdObj  QshCmd;

void qsh_recv(char recv);

#ifdef QSH_USING_LIBC
int qsh_export(const char *name, int (*handle)(int, char**), const char *usage);
#endif

void qsh_init(void);

void qsh_exec(void);

void qsh_cmd_init(QshCmd *qcmd, const char *name, int (*handle)(int, char**), const char *usage);

void qsh_cmd_add(QshCmd* qcmd);

void qsh_cmd_del(QshCmd* qcmd);

#ifdef __cplusplus
 }
#endif

#endif