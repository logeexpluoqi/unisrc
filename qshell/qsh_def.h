/**
 * @ Author: luoqi
 * @ Create Time: 2023-12-07 20:39
 * @ Modified by: luoqi
 * @ Modified time: 2023-12-11 19:51
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

#define QTERM_SU(n)             "\033["#n"S"   // scroll up
#define QTERM_SD(n)             "\033["#n"T"   // scroll down
#define QTERM_CUU(n)            "\033["#n"A"   // cursor up
#define QTERM_CUD(n)            "\033["#n"B"   // cursor down
#define QTERM_CUF(n)            "\033["#n"C"   // cursor front
#define QTERM_CUB(n)            "\033["#n"D"   // cursor back
#define QTERM_ICH(n)            "\033["#n"@"   // insert charactor
#define QTERM_DCH(n)            "\033["#n"P"   // delete charactor
#define QTERM_ECH(n)            "\033["#n"X"   // erase charactor
#define QTERM_IL(n)             "\033["#n"L"   // inset line
#define QTERM_DL(n)             "\033["#n"M"   // delete line
#define QTERM_CBL_ON            "\033[?12h"    // cursor blink on
#define QTERM_CBL_OFF           "\033[?12l"    // cursor blink off
#define QTERM_CDISP_ON          "\033[1?25h"   // cursor display on
#define QTERM_CDISP_OFF         "\033[1?25l"   // cursor display off
#define QTERM_CSAP_USR          "\033[0SPq"    // cursor shape user
#define QTERM_CSAP_BB           "\033[1SPq"    // cursor shape blinking block
#define QTERM_CSAP_BBAR         "\033[5SPq"    // cursor shape blinking bar
#define QTERM_CSAP_SBAR         "\033[6SPq"    // cursor shape steady bar

#endif
