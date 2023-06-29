/*
 * @Author: luoqi 
 * @Date: 2022-11-22 22:42:33 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-28 00:00:26
 */

#include "qdemo.h"
#include "../qmath/qtrigf.h"
#include "../qshell/qsh.h"

static CmdObj cmd_qtrigf;
static int cmd_qtrif_hdl(int argc, char **argv);

int demo_qtrigf_init()
{
    qcmd_init(&cmd_qtrigf, "qtrigf", cmd_qtrif_hdl, "@ sin, cos, tan, asin, acos, atan");
    qcmd_add(&cmd_qtrigf);
    return 0;
}

static int qsin_func()
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

static int qcos_func()
{
    int p = 1;
    for(int i = 0; i < 36000; i++){
        if(p++ > 10){
            printf("\r\n");
            p = 2;
        }
        printf(" %+f", qfcosd(i * 0.01));
    }
    printf("\r\n");
    return 0;
}

int cmd_qtrif_hdl(int argc, char **argv)
{
    if(argc != 2){
        return CMD_PARAM_ERR;
    }
    if(ISARG(argv[1], "sin")){
        qsin_func();
    }else if(ISARG(argv[1], "cos")){
        qcos_func();
    }else if(ISARG(argv[1], "tan")){
        
    }else if(ISARG(argv[1], "asin")){
        
    }else if(ISARG(argv[1], "acos")){
        
    }else if(ISARG(argv[1], "atan")){
        
    }else{
        return CMD_PARAM_ERR;
    }

    return CMD_NO_ERR;   
}
