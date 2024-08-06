/*
 * @Author: luoqi
 * @Date: 2021-05-26 16:10:26
 * @ Modified by: luoqi
 * @ Modified time: 2024-08-06 21:12
 */

#include <stdlib.h>
#include <string.h>
#include "qterm.h"

static QCliInterface qshell;

int isarg(const char *s, const char *arg);

int qterm_init(void)
{
    return qcli_init(&qshell, printf);
}

int qterm_exec(char c)
{
    return qcli_exec(&qshell, c);
}

int qterm_call(char *args)
{
    return qcli_exec_str(&qshell, args);
}

int qterm_attach(QCliCmd *cmd, const char *name, QCliCallback callback, const char *usage)
{
    return qcli_add(&qshell, cmd, name, callback, usage);
}

int qterm_detach(QCliCmd *cmd)
{
    return qcli_remove(&qshell, cmd);
}

int isarg(const char *s, const char *arg)
{
    return strcmp(s, arg);
}

int qcmd_create(const char *name, QCliCallback callback, const char *usage)
{
    QCliCmd *cmd = (QCliCmd *)malloc(sizeof(QCliCmd));
    if (cmd == NULL) {
        return -1;
    } else {
        qcli_add(&qshell, cmd, name, callback, usage);
    }
    return 0;
}
