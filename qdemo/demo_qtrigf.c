/*
 * @Author: luoqi 
 * @Date: 2022-11-22 22:42:33 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-22 23:57:10
 */

#include "demo_qtrigf.h"
#include "../qmath/qtrigf.h"
#include "../qshell/qsh.h"

static CmdObj cmd_qtrigf;
static int cmd_qtrif_hdl(int argc, char **argv);

int demo_qtrigf_init()
{
    qcmd_init(&cmd_qtrigf, "qtrigf", cmd_qtrif_hdl, "@ ");
    qcmd_add(&cmd_qtrigf);
    return 0;
}

static int qtrif_func()
{
    int p = 1;
    for(int i = 0; i < 36000; i++){
        if(p++ > 10){
            printf("\r\n");
            p = 2;
        }
        printf(" %+f", qfsind(i * 0.01));
    }
    printf("\r\n");
    return 0;
}

int cmd_qtrif_hdl(int argc, char **argv)
{
    if(argc != 1){
        return CMD_PARAM_ERR;
    }
    qtrif_func();

    return CMD_NO_ERR;   
}
