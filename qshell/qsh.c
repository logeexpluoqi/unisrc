/*
 * @Author: luoqi
 * @Date: 2021-05-26 16:10:26
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-25 00:40:28
 */

#include <stdlib.h>
#include <stdint.h>
#include "qsh.h"

#define QSH_NULL    ((void*)0)

#define QSH_USAGE_DISP_MAX  80

enum {
    QSH_KEY_BACKSPACE = '\b',
    QSH_KEY_SPACE = '\20',
    QSH_KEY_ENTER = '\r',
    QSH_KEY_UP = '\x41',
    QSH_KEY_DOWN = '\x42',
    QSH_KEY_RIGHT = '\x43',
    QSH_KEY_LEFT = '\x44',
};

typedef enum {
    QSH_RECV_UNFINISH,
    QSH_RECV_FINISH
} RecvState;

typedef enum {
    QSH_FUNCKEY_NONE = 0,
    QSH_FUNCKEY_START,
    QSH_FUNCKEY_RECV,
} QshFunckeyStage;

#define QSH_CLEAR_LINE      QPRINTF("\r\x1b[K")
#define QSH_SHOW_PERFIX     QPRINTF("\r%s", _perfix)
#define QSH_CLEAR_DISP      QPRINTF("\033[H\033[2J")

static const char *_perfix = "/>$ ";
static char _args[CMD_MAX_LEN];
static uint32_t _args_c = 0;
static char _hs_buf[QSH_HISTORY_MAX][CMD_MAX_LEN];
static uint32_t hs_cnt = 0;
static uint32_t hs_recall = 0;
static uint32_t hs_index = 0;
static RecvState recv_state = QSH_RECV_UNFINISH;

static CmdObj _history;
static CmdObj _clear;
static CmdObj _help;

static int _help_hdl(int argc, char **argv);

static int _hs_hdl(int argc, char **argv);

static int _clear_hdl(int argc, char **argv);

static void *_memcpy(void *dest, const void *src, uint32_t len)
{
    char *d;
    const char *s;
    if((dest > (src + len)) || (dest < src)) {
        d = dest;
        s = src;
        while(len--) {
            *d++ = *s++;
        }
    } else {
        d = (char *)(dest + len - 1);
        s = (char *)(src + len - 1);
        while(len--) {
            *d-- = *s--;
        }
    }
    return dest;
}

static void *_memset(void *dest, int c, uint32_t n)
{
    if((dest == QSH_NULL) || n < 0) {
        return QSH_NULL;
    } else {
        char *pdest = (char *)dest;
        while(n--) {
            *pdest++ = c;
        }
        return dest;
    }
}

static uint32_t _strlen(const char *s)
{
    uint32_t len = 0;
    while(*s++ != '\0') {
        len++;
    }
    return len;
}

static char *_strcpy(char *dest, const char *src)
{
    if((dest == QSH_NULL) || (src == QSH_NULL)) {
        return QSH_NULL;
    }
    char *addr = dest;
    while((*dest++ = *src++) != '\0');
    return addr;
}

int _strcmp(const char *s1, const char *s2)
{
    uint32_t i = 0;

    while((*(s1 + i) != '\0') || (*(s2 + i) != '\0')) {
        if(*(s1 + i) != *(s2 + i)) {
            return 1;
        } else {
            i++;
        }
    }
    return 0;
}

void qsh_init()
{
    _memset(_args, 0, sizeof(_args));
    _memset(_hs_buf, 0, sizeof(_hs_buf));
    hs_index = 0;
    hs_cnt = 0;
    hs_recall = 0;
    recv_state = QSH_RECV_UNFINISH;
    _args_c = 0;
    cmd_init(&_history, "hs", _hs_hdl, "show history");
    cmd_init(&_help, "?", _help_hdl, "do help");
    cmd_init(&_clear, "clear", _clear_hdl, "clear window");

    cmd_add(&_help);
    cmd_add(&_history);
    cmd_add(&_clear);
    QSH_CLEAR_DISP;
    QPRINTF("========== QSH BY LUOQI ==========\r\n");
    QSH_SHOW_PERFIX;
}

int qsh_call(const char *args)
{
    char _args[CMD_MAX_LEN + 1] = { 0 };
    _strcpy(_args, args);
    return cmd_exec(_args);
}

static inline void _recv_enter()
{
    if(_args_c != 0) {
        /* clear space charactors behind args */
        while(*(_args + _args_c) == QSH_KEY_SPACE) {
            *(_args + _args_c) = 0;
            _args_c--;
        }
        /* save history */
        if(_strcmp(_args, "hs") != 0) {
            /* if this arg and last arg are different, save this arg */
            if(_strcmp(_args, _hs_buf[(hs_index + QSH_HISTORY_MAX - 1) % QSH_HISTORY_MAX]) != 0) {
                _memcpy(_hs_buf[hs_index], _args, _strlen(_args));
                hs_index = (hs_index + 1) % QSH_HISTORY_MAX;
                hs_cnt = (hs_cnt + 1 > QSH_HISTORY_MAX) ? QSH_HISTORY_MAX : (hs_cnt + 1);
            }
        }
        recv_state = QSH_RECV_FINISH;
    } else {
        QPRINTF("\r\n");
        QSH_SHOW_PERFIX;
    }
}

static inline void _prev_history()
{
    uint32_t index = ((hs_index + QSH_HISTORY_MAX - 1) - hs_recall) % QSH_HISTORY_MAX;
    _args_c = _strlen(_hs_buf[index]) + 1;
    _memcpy(_args, _hs_buf[index], _args_c);
    QPRINTF("%s", _args);
}

static inline void _next_history()
{
    uint32_t index = ((hs_index + QSH_HISTORY_MAX - 1) - hs_recall) % QSH_HISTORY_MAX;
    _args_c = _strlen(_hs_buf[index]) + 1;
    _memcpy(_args, _hs_buf[index], _args_c);
    QPRINTF("%s", _args);

}

static inline void _recv_up()
{
    if(hs_recall < hs_cnt) {
        QSH_CLEAR_LINE;
        QSH_SHOW_PERFIX;
        _prev_history();
        hs_recall ++;
    }
}

static inline void _recv_down()
{
    if(hs_recall > 1) {
        QSH_CLEAR_LINE;
        QSH_SHOW_PERFIX;
        _next_history();
        hs_recall --;
    } else {
        QSH_CLEAR_LINE;
        QSH_SHOW_PERFIX;
    }
}

static inline void _recv_right()
{

}

static inline void _recv_left()
{

}

static inline void _recv_characters(char c)
{
    if(c == QSH_KEY_ENTER) {
        _recv_enter();
    } else if(c == QSH_KEY_BACKSPACE) {
        if(_args_c > 0) {
            _args_c--;
            _args[_args_c] = 0;
            QPRINTF("\b \b");
        }
    } else {
        _args[_args_c] = c;
        _args_c++;
        QPRINTF("%c", c);
    }
}

void qsh_recv(char c)
{
    static QshFunckeyStage key_state = QSH_FUNCKEY_NONE;
    /* functional key receive FSM */
    switch(key_state) {
    case QSH_FUNCKEY_NONE:
        if(c == '\x1b') {
            key_state = QSH_FUNCKEY_START;
        } else {
            _recv_characters(c);
            key_state = QSH_FUNCKEY_NONE;
        }
        break;
    case QSH_FUNCKEY_START:
        if(c == '\x5b') {
            key_state = QSH_FUNCKEY_RECV;
        } else {
            key_state = QSH_FUNCKEY_NONE;
        }
        break;
    case QSH_FUNCKEY_RECV:
        if(c == QSH_KEY_UP) {
            _recv_up();
        } else if(c == QSH_KEY_DOWN) {
            _recv_down();
        } else if(c == QSH_KEY_RIGHT) {
            _recv_right();
        } else if(c == QSH_KEY_LEFT) {
            _recv_left();
        } else {
            ;
        }
        key_state = QSH_FUNCKEY_NONE;
        break;
    default:
        key_state = QSH_FUNCKEY_NONE;
        break;
    }
}

static inline void _reset()
{
    _memset(_args, 0, _args_c);
    hs_recall = 0;
    _args_c = 0;
}

void qsh_exec()
{
    if(recv_state == QSH_RECV_FINISH) {
        QPRINTF("\r\n");
        switch(cmd_exec(_args)) {
        case CMD_EOK:
            break;
        case CMD_LENGTH_OUT:
            QPRINTF(" #! length exceed !\r\n");
            break;
        case CMD_MISSING:
            QPRINTF(" #! cmd missing !\r\n");
            break;
        case CMD_PARAM_OVERFLOW:
            QPRINTF(" #! param overflow !\r\n");
            break;
        case CMD_PARAM_LESS:
            QPRINTF(" #! param short !\r\n");
            break;
        case CMD_EXEC_ERR:
            QPRINTF(" #! exec error !\r\n");
            break;
        case CMD_PARAM_ERR:
            QPRINTF(" #! param error !\r\n");
            break;
        default:
            break;
        }
        _reset();
        recv_state = QSH_RECV_UNFINISH;
        QSH_SHOW_PERFIX;
    }
}

void qcmd_init(CmdObj *qcmd, const char *name, int (*handle)(int, char **), const char *usage)
{
    cmd_init((CmdObj *)qcmd, name, handle, usage);
}

int qcmd_export(const char *name, int (*handle)(int, char **), const char *usage)
{
    CmdObj *qcmd = (CmdObj *)malloc(sizeof(CmdObj));
    cmd_init((CmdObj *)qcmd, name, handle, usage);
    cmd_add(qcmd);
}

void qcmd_add(CmdObj *qcmd)
{
    cmd_add((CmdObj *)qcmd);
}

void qcmd_del(CmdObj *qcmd)
{
    cmd_del((CmdObj *)qcmd);
}

int _hs_hdl(int argc, char **argv)
{
    for(int i = hs_cnt; i > 0; i--) {
        QPRINTF(" %2d: %s\r\n", i, _hs_buf[(hs_index - i + QSH_HISTORY_MAX) % QSH_HISTORY_MAX]);
    }
    return 0;
}

int _clear_hdl(int argc, char **argv)
{
    QSH_CLEAR_DISP;
    return 0;
}

int _help_hdl(int argc, char **argv)
{
    CmdObj *_cmd;
    int i, j, k = 0;
    int len;
    uint32_t num = cmd_num();
    QPRINTF("  Number:          %d\r\n", num);
    QPRINTF("  Commands       Usage \r\n");
    QPRINTF(" ----------     -------\r\n");
    for(i = 1; i <= num; i++) {
        _cmd = cmd_obj(i);
        QPRINTF(" - %-9s     > ", _cmd->name);
        len = _strlen(_cmd->usage);
        if(len < QSH_USAGE_DISP_MAX) {
            QPRINTF("%s\r\n", _cmd->usage);
        } else {
            while(len > QSH_USAGE_DISP_MAX) {
                k++;
                len = len - QSH_USAGE_DISP_MAX;
            }

            for(j = 0; j <= k; j++) {
                if(j == 0) {
                    QPRINTF("%-80.80s\r\n", _cmd->usage + j * QSH_USAGE_DISP_MAX);
                } else {
                    QPRINTF("                   %-80.80s\r\n", _cmd->usage + j * QSH_USAGE_DISP_MAX);
                }
            }
        }
        k = 0;
    }
    return CMD_EOK;
}
