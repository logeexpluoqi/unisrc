/**
 * @ Author: luoqi
 * @ Create Time: 2023-12-07 20:39
 * @ Modified by: luoqi
 * @ Modified time: 2023-12-07 22:19
 * @ Description:
 */

#ifndef _QSH_DEF_H
#define _QSH_DEF_H

#include <stdio.h>
#include <stdint.h>

#define QSH_USING_LIBC

#define QSH_CMD_LEN_MAX         30
#define QSH_CMD_ARGS_MAX        10
#define QSH_HISTORY_MAX         10

#define QSH_USAGE_DISP_MAX      80

#define QSH_PERFIX              "/>$ "

#define QSH(...)                printf(__VA_ARGS__)

#define QPRINTF(...)            printf(__VA_ARGS__);

#define QSH_KEY_BACKSPACE       '\b'
#define QSH_KEY_SPACE           '\20'
#define QSH_KEY_ENTER           '\r'
#define QSH_KEY_UP              '\x41'
#define QSH_KEY_DOWN            '\x42'
#define QSH_KEY_RIGHT           '\x43'
#define QSH_KEY_LEFT            '\x44'

#define ASCII_SPACE             '\x20'
#define ASCII_DOT               '\x2c'

#endif
