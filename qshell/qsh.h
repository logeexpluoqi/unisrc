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

#include "qsh_def.h"

#define ISARG(str1, str2)       (qcmd_isarg(str1, str2) == 0)

typedef struct _clist {
    struct _clist *prev;
    struct _clist *next;
} QCList;

typedef int (*CmdCallback)(int, char **);
typedef struct
{
    const char      *name;
    uint32_t        id;
    CmdCallback     callback;
    const char      *usage;
    QCList          node;
} QCmdObj;

int qcmd_isarg(const char *s, const char *arg);

int qsh_recv(char c);

int qsh_init(void);

int qsh_exec(void);

int qcmd_init(QCmdObj *qcmd, const char *name, int (*handle)(int, char **), const char *usage);

int qcmd_add(QCmdObj *qcmd);

int qcmd_del(QCmdObj *qcmd);

int qsh_call(const char *args);

#ifdef QSH_USING_LIBC
int qcmd_export(const char *name, int (*handle)(int, char **), const char *usage);
#endif

#ifdef __cplusplus
}
#endif

#endif
