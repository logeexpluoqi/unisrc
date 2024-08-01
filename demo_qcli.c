/**
 * @ Author: luoqi
 * @ Create Time: 2024-08-02 03:24
 * @ Modified by: luoqi
 * @ Modified time: 2024-08-02 03:38
 * @ Description:
 */


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "qshell/qcli.h"

static QCliInterface cli;

int main()
{
    qcli_init(&cli, printf);
    char ch;
    for (;;) {
        if(system("stty raw -echo") < 0){
            printf(" #! system call error !\r\n");
        }
        ch = getchar();
        ch = (ch == 127) ? 8 : ch;
        if (ch != 3) {
            qcli_exec(&cli, ch);
        } else {
            if(system("stty -raw echo") < 0){
                printf(" #! system call error !\r\n");
            }
            printf("\33[2K");
            printf("\033[H\033[J");
            printf(" \r\n#! qsh input thread closed !\r\n\r\n");
            return 0;
        }
    }
    return 0;
}
