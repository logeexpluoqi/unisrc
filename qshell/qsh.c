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

enum{
    QSH_KEY_BACKSPACE   = '\b',
    QSH_KEY_SPACE       = '\20',
    QSH_KEY_ENTER       = '\r',
    QSH_KEY_UP          = '\x41',
    QSH_KEY_DOWN        = '\x42',
    QSH_KEY_RIGHT       = '\x43',
    QSH_KEY_LEFT        = '\x44',
};

typedef enum {
    RECV_UNFINISH,
    RECV_FINISH
} RecvState;

typedef enum{
    QSH_RECALL_PREV = -1,
    QSH_RECALL_NULL = 0,
    QSH_RECALL_NEXT = 1,
} QshRecallState;

#define QSH_CLEAR_LINE      QPRINTF("\r\x1b[K")
#define QSH_SHOW_PERFIX     QPRINTF("\r%s", _perfix)
#define QSH_CLEAR           QPRINTF("\033[H\033[J")

static const char *_perfix = "/>$ ";
static char _args[CMD_MAX_LEN];
static char hs_buf[QSH_HISTORY_MAX][CMD_MAX_LEN];
static unsigned int hs_cnt = 0;
static unsigned int recall_cnt = 0;
static unsigned int recv_index = 0;
static unsigned int hs_index = 0;
static unsigned int recall_index = 0;
static QshRecallState recall_status = QSH_RECALL_NULL;
static RecvState recv_state = RECV_UNFINISH;

static inline void _prev_history(void);

static inline void _next_history(void);

static inline void _recv_backspace(void);

static inline void _recv_enter(void);

static inline void _recv_up(void);

static inline void _recv_down(void);

static inline void _recv_right(void);

static inline void _recv_left(void);

static CmdObj _hs;
static CmdObj _clear;
static CmdObj _help;

static int _help_hdl(int argc, char **argv);

static int _hs_hdl(int argc, char **argv);

static int _clear_hdl(int argc, char **argv);

static void *_memcpy(void *dst, void *src, uint32_t len)
{
    char *d;
    const char *s;
    if((dst > (src + len)) || (dst < src)) {
        d = dst;
        s = src;
        while(len--) {
            *d++ = *s++;
        }
    } else {
        d = (char *)(dst + len - 1);
        s = (char *)(src + len - 1);
        while(len--) {
            *d-- = *s--;
        }
    }
    return dst;
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
    _memset(hs_buf, 0, sizeof(hs_buf));
    hs_index = 0;
    hs_cnt = 0;
    recall_index = 0;
    recall_cnt = 0;
    recall_status = QSH_RECALL_NULL;
    recv_state = RECV_UNFINISH;
    recv_index = 0;

    cmd_init(&_hs, "hs", _hs_hdl, "show history");
    cmd_init(&_help, "?", _help_hdl, "do help");
    cmd_init(&_clear, "clear", _clear_hdl, "clear window");

    cmd_add(&_help);
    cmd_add(&_hs);
    cmd_add(&_clear);
    QSH_CLEAR;
    QPRINTF("========== QSH BY LUOQI ==========\r\n");
    QSH_SHOW_PERFIX;
}

int qsh_call(const char *args)
{
    char _args[CMD_MAX_LEN + 1] = { 0 };
    _strcpy(_args, args);
    return cmd_exec(_args);
}

void _recv_enter()
{
    int h_index;
    if(recv_index != 0) {
        while(*(_args + recv_index) == QSH_KEY_SPACE){
            *(_args + recv_index) = 0;
            recv_index --;
        }
        /* save history */
        if(_strcmp(_args, "hs") != 0) {
            h_index = (hs_index + QSH_HISTORY_MAX - 1) % QSH_HISTORY_MAX;
            if(_strcmp(_args, hs_buf[h_index]) != 0) {
                _memcpy(hs_buf[hs_index], _args, _strlen(_args));
                hs_index = (hs_index + 1) % QSH_HISTORY_MAX;
                hs_cnt = (hs_cnt + 1 > QSH_HISTORY_MAX) ? QSH_HISTORY_MAX : (hs_cnt + 1);
            }
        }

        /* history recall record */
        if(recall_status == QSH_RECALL_NEXT) {
            if(recall_cnt != hs_cnt) {
                recall_cnt--;
            } else {
                recall_cnt = hs_cnt;
            }
        } else if(recall_status == QSH_RECALL_PREV) {
            if(recall_cnt != 0) {
                recall_cnt++;
            } else {
                recall_cnt = 0;
            }
        }else{
            recall_status = QSH_RECALL_NULL;
        }

        recall_index = hs_index;
        recall_cnt = 0;
        recv_index = 0;
        QPRINTF("\r\n");
        recv_state = RECV_FINISH;
    } else {
        QPRINTF("\r\n");
        QSH_SHOW_PERFIX;
    }
}

void _prev_history()
{
    recall_status = QSH_RECALL_PREV;
    if(recall_cnt < hs_cnt) {
        recall_index = (recall_index - 1 + QSH_HISTORY_MAX) % QSH_HISTORY_MAX;
        recall_cnt++;
    } else {
        recall_cnt = hs_cnt;
    }
    recv_index = _strlen(hs_buf[recall_index]);
    _memcpy(_args, hs_buf[recall_index], recv_index);
}

void _next_history()
{
    recall_status = QSH_RECALL_NEXT;
    if(recall_cnt > 0) {
        recall_index = (recall_index + 1) % QSH_HISTORY_MAX;
        recv_index = _strlen(hs_buf[recall_index]);
        _memcpy(_args, hs_buf[recall_index], recv_index);
        recall_cnt--;
    } else {
        recall_cnt = 0;
        _memset(_args, 0, _strlen(_args));
    }
}

void _recv_backspace()
{
    QPRINTF("\b ");
    _args[--recv_index] = 0;
    QSH_SHOW_PERFIX;
    QPRINTF("%s", _args);
}

void _recv_up()
{
    if(hs_cnt > 0) {
        QSH_CLEAR_LINE;
        QSH_SHOW_PERFIX;
        _prev_history();
        QPRINTF("%s", _args);
    } else {
        QSH_CLEAR_LINE;
        QSH_SHOW_PERFIX;
    }
}

void _recv_down()
{
    if(hs_cnt > 0) {
        QSH_CLEAR_LINE;
        QSH_SHOW_PERFIX;
        _next_history();
        QPRINTF("%s", _args);
    } else {
        QSH_CLEAR_LINE;
        QSH_SHOW_PERFIX;
    }
}

void _recv_right()
{

}

void _recv_left()
{

}

void qsh_recv(char c)
{
    if((c == '\x1b') || (c == '\x5b')
        || (c == QSH_KEY_UP) || (c == QSH_KEY_DOWN) || (c == QSH_KEY_RIGHT) || (c == QSH_KEY_LEFT)) {
        switch(c) {
        case QSH_KEY_UP: _recv_up(); break;
        case QSH_KEY_DOWN: _recv_down(); break;
        case QSH_KEY_RIGHT: _recv_right(); break;
        case QSH_KEY_LEFT: _recv_left(); break;
        default: break;
        }
    } else if(c == QSH_KEY_ENTER) {
        _recv_enter();
    } else if(c == QSH_KEY_BACKSPACE && recv_index > 0) {
        _recv_backspace();
    } else {
        _args[recv_index++] = c;
        QPRINTF("%c", c);
    }
}

void qsh_exec()
{
    if(recv_state == RECV_FINISH) {
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
        recv_index = 0;
        _memset(_args, 0, _strlen(_args));
        QPRINTF("%s", _perfix);
        recv_state = RECV_UNFINISH;
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
    char hs_pos;
    if((hs_index - hs_cnt) >= 0) {
        hs_pos = hs_index - hs_cnt;
    } else {
        hs_pos = hs_index;
    }

    for(int i = 0; i < hs_cnt; i++) {
        QPRINTF(" %2d: %s\r\n", hs_cnt - i, hs_buf[(hs_pos + i) % QSH_HISTORY_MAX]);
    }
    return 0;
}

int _clear_hdl(int argc, char **argv)
{
    QSH_CLEAR;
    return 0;
}

int _help_hdl(int argc, char **argv)
{
    CmdObj *_cmd;
    int i, j, k = 0;
    int len;
    unsigned int num = cmd_num();
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
