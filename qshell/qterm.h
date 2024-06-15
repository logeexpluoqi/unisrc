/*
 * @Author: luoqi 
 * @Date: 2021-05-26 16:10:33 
 * @ Modified by: luoqi
 * @ Modified time: 2024-06-15 22:16
 */

#ifndef _QTERM_H
#define _QTERM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qterm_def.h"

#define ISARG(str1, str2)       (isarg(str1, str2) == 0)

typedef struct _clist {
    struct _clist *prev;
    struct _clist *next;
} QCList;

typedef int (*CmdHandle)(int, char **);
typedef struct
{
    const char      *name;
    uint32_t        id;
    CmdHandle       handle;
    const char      *usage;
    QCList          node;
} QCmdObj;

int isarg(const char *s, const char *arg);

int qterm_init(void);

int qterm_exec(char c);

int qterm_call(const char *args);

int qterm_attatch(QCmdObj *qcmd, const char *name, CmdHandle handle, const char *usage);

int qterm_detach(QCmdObj *qcmd);

#ifdef QSH_USING_LIBC
int qcmd_create(const char *name, CmdHandle handle, const char *usage);
#endif

#ifdef __cplusplus
}
#endif

#endif
