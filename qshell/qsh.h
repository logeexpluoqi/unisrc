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

#define QSH_HISTORY_MAX         10

#define QSH(...)                printf(__VA_ARGS__)

#define ISARG(str1, str2)       (strcmp(str1, str2) == 0)

void qsh_recv(char recv);

void qsh_init(void);

void qsh_exec(void);

void qcmd_init(CmdObj *qcmd, const char *name, int (*handle)(int, char **), const char *usage);

void qcmd_add(CmdObj *qcmd);

void qcmd_del(CmdObj *qcmd);

int qcmd_call(const char *args);

#ifdef QSH_USING_LIBC

int qcmd_export(const char *name, int (*handle)(int, char **), const char *usage);

#endif

#ifdef __cplusplus
}
#endif

#endif
