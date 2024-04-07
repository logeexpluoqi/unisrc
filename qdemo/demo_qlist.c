/**
 * @ Author: luoqi
 * @ Create Time: 2024-04-07 23:15
 * @ Modified by: luoqi
 * @ Modified time: 2024-04-07 23:27
 * @ Description:
 */

#include "qdemo.h"
#include "../qlib/qlist.h"
#include "../qshell/qsh.h"

static QLIST_CREATE(list);

static QLIST_CREATE(node1);
static QLIST_CREATE(node2);
static QLIST_CREATE(node3);

static int _cmd_qlist_hdl(int argc, char **argv);

int demo_qlist_init(void)
{
    qlist_insert_after(&list, &node1);
    qlist_insert_after(&list, &node2);

    qcmd_create("qlist", _cmd_qlist_hdl, "qlist command");
}

int _cmd_qlist_hdl(int argc, char **argv)
{
    QSH(" node1: %d\r\n", qlist_isexist(&list, &node1));
    QSH(" node2: %d\r\n", qlist_isexist(&list, &node2));
    QSH(" node3: %d\r\n", qlist_isexist(&list, &node3));
    return 0;
}
