/*
 * @Author: luoqi 
 * @Date: 2021-05-26 16:10:33 
 * @ Modified by: luoqi
 * @ Modified time: 2024-08-06 21:12
 */

#ifndef _QTERM_H
#define _QTERM_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include "qcli.h"

#define QSH_USING_LIBC

#define QSH(...)                printf(__VA_ARGS__)
#define ISARG(str1, str2)       (isarg(str1, str2) == 0)

int isarg(const char *s, const char *arg);

int qterm_init(void);

int qterm_exec(char c);

int qterm_call(char *args);

int qterm_attach(QCliCmd *cmd, const char *name, QCliCallback callback, const char *usage);

int qterm_detach(QCliCmd *cmd);

#endif


#ifdef QSH_USING_LIBC
int qcmd_create(const char *name, QCliCallback callback, const char *usage);
#endif

#ifdef __cplusplus
}
#endif
