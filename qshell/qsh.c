/*
 * @Author: luoqi
 * @Date: 2021-05-26 16:10:26
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-25 00:40:28
 */

#include "qsh.h"

#ifdef QSH_USING_LIBC
#include <stdlib.h>
#endif

#define QSH_NULL    ((void*)0)

typedef enum {
    QSH_FUNCKEY_NONE = 0,
    QSH_FUNCKEY_START,
    QSH_FUNCKEY_RECV,
} QshFunckeyStage;

typedef enum {
    QCMD_EXEC_ERR       = -2,
    QCMD_PARAM_ERR      = -1,
    QCMD_EOK            = 0,
    QCMD_MISSING        = 1,
    QCMD_PARAM_OVERFLOW = 2,
    QCMD_PARAM_LESS     = 3,
} QCmdInfo;

#define QSH_CLEAR_LINE      QPRINTF("\r\x1b[K")
#define QSH_SHOW_PERFIX     QPRINTF("\r%s", QSH_PERFIX)
#define QSH_CLEAR_DISP      QPRINTF("\033[H\033[2J")

#define QCLIST_OBJ(ptr, type, member)     ((type *)((char *)(ptr) - ((unsigned long) &((type*)0)->member)))
#define QCLIST_ITERATOR(node, clist)      for (node = (clist)->next; node != (clist); node = node->next)

static QCList clist = { &clist, &clist };
static uint32_t id = 1;

static int iscall = 0;
static int isenter = 0;

static char _args[QSH_CMD_LEN_MAX];
static uint32_t _args_c = 0;
static uint32_t _cur_index = 0;
static char _hs_buf[QSH_HISTORY_MAX][QSH_CMD_LEN_MAX];
static uint32_t hs_cnt = 0;
static uint32_t hs_recall = 0;
static uint32_t hs_index = 0;

static QCmdObj _history;
static QCmdObj _clear;
static QCmdObj _help;

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

static int _strcmp(const char *s1, const char *s2)
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

static void *_strinsert(char *s, uint32_t offset, char *c, uint32_t size)
{
    uint32_t len = _strlen(s);
    _memcpy(s + offset + size, s + offset, len - offset + 1);
    _memcpy(s + offset, c, size);
    return s;
}

static void *_strdelete(char *s, uint32_t offset, uint32_t size)
{
    uint32_t len = _strlen(s);
    _memcpy(s + offset, s + offset + size, len - offset - size);
    _memset(s + len - size, 0, size);
    return s;
}

static inline void _clist_insert(QCList *list, QCList *node)
{
    list->next->prev = node;
    node->next = list->next;

    list->next = node;
    node->prev = list;
}

static inline void _cqlist_remove(QCList *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;

    node->next = node->prev = node;
}

static QCmdInfo _cmd_exec(char *args)
{
    QCList *_node;
    QCmdObj *cmd;
    char *argv[QSH_CMD_ARGS_MAX + 1] = { 0 };
    int argc = 0;
    int argn = 0;
    int i = 0;

    /* cmd parser */
    do {
        if(i == 0) {
            argv[argn] = args + i;
        } else if(args[i] == ASCII_SPACE || args[i] == ASCII_DOT) {
            args[i] = 0;
            if(args[i + 1] != ASCII_SPACE && args[i + 1] != ASCII_DOT) {
                argv[++argn] = args + i + 1;
            }
        }
        i++;
        if(argn > QSH_CMD_ARGS_MAX - 1) {
            return QCMD_PARAM_OVERFLOW;
        }

    } while(args[i] != 0);
    /* End of dismantling input command string */
    /* add 1 because there is no space key value front of the first argv */
    argc = argn + 1;
    QCLIST_ITERATOR(_node, &clist) {
        cmd = QCLIST_OBJ(_node, QCmdObj, node);
        if(_strcmp(cmd->name, argv[0]) == 0) {
            return cmd->callback(argc, argv);
        }
    }
    return QCMD_MISSING;
}

static int _cmd_isexist(QCmdObj *cmd)
{
    QCList *_node;
    QCmdObj *_cmd;

    QCLIST_ITERATOR(_node, &clist) {
        _cmd = QCLIST_OBJ(_node, QCmdObj, node);
        if(cmd->id == _cmd->id) {
            return 1;
        } else {
            continue;
        }
    }
    return 0;
}

static uint32_t _cmd_num()
{
    unsigned int len = 0;
    const QCList *l = &clist;
    while (l->next != &clist) {
        l = l->next;
        len++;
    }

    return len;
}

static QCmdObj *_cmd_obj(uint32_t id)
{
    QCList *_node = &clist;

    for(uint32_t i = id; i > 0; i--) {
        _node = _node->next;
    }

    return QCLIST_OBJ(_node, QCmdObj, node);
}

static inline void _recv_enter()
{
    if(_args_c != 0) {
        /* clear space charactors behind args */
        while(*(_args + _args_c) == QSH_KEY_SPACE) {
            *(_args + _args_c) = 0;
            _args_c--;
            _cur_index = _args_c;
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
        isenter = 1;
    } else {
        QPRINTF("\r\n");
        QSH_SHOW_PERFIX;
    }
}

static inline void _prev_history()
{
    uint32_t index = ((hs_index + QSH_HISTORY_MAX - 1) - hs_recall) % QSH_HISTORY_MAX;
    _args_c = _strlen(_hs_buf[index]) ;
    _cur_index = _args_c;
    _memcpy(_args, _hs_buf[index], _args_c);
    QPRINTF("%s", _args);
}

static inline void _next_history()
{
    uint32_t index = ((hs_index + QSH_HISTORY_MAX - 1) - hs_recall) % QSH_HISTORY_MAX;
    _args_c = _strlen(_hs_buf[index]);
    _cur_index = _args_c;
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
        _memset(_args, 0, _args_c);
        _args_c = 0;
        _cur_index = _args_c;
        QSH_CLEAR_LINE;
        QSH_SHOW_PERFIX;
    }
}

static inline void _recv_right()
{
    if(_cur_index < _args_c){
        QPRINTF(QTERM_CUF(1));
        _cur_index ++;
    }
}

static inline void _recv_left()
{
    if(_cur_index > 0){
        QPRINTF(QTERM_CUB(1));
        _cur_index --;
    }
}

static inline void _recv_characters(char c)
{
    if(c == QSH_KEY_ENTER) {
        _recv_enter();
    } else if(c == QSH_KEY_BACKSPACE) {
        if((_args_c == _cur_index) && (_args_c > 0)) {
            _args_c--;
            _cur_index = _args_c;
            _args[_args_c] = 0;
            QPRINTF("\b \b");
        }else if(_cur_index > 0){
            _cur_index --;
            _strdelete(_args, _cur_index, 1);
            _args_c --;
            QPRINTF(QTERM_CUB(1));
            QPRINTF(QTERM_DCH(1));
        }else{
            return;
        }
    } else if(_args_c < QSH_CMD_LEN_MAX) {
        if(_args_c == _cur_index){
            _args[_args_c] = c;
            _args_c++;
            _cur_index = _args_c;
            QPRINTF("%c", c);
        }else {
            _strinsert(_args, _cur_index, &c, 1);
            _args_c ++;
            _cur_index ++;
            QPRINTF(QTERM_ICH(1));
            QPRINTF("%c", c);
        }
    } else {
        return;
    }
}

static inline void _reset()
{
    _memset(_args, 0, _args_c);
    hs_recall = 0;
    _args_c = 0;
    _cur_index = _args_c;
}

int qsh_init()
{
    _memset(_args, 0, sizeof(_args));
    _memset(_hs_buf, 0, sizeof(_hs_buf));
    hs_index = 0;
    hs_cnt = 0;
    hs_recall = 0;
    isenter = 0;
    iscall = 0;
    _args_c = 0;
    _cur_index = _args_c;
    qcmd_init(&_history, "hs", _hs_hdl, "show history");
    qcmd_init(&_help, "?", _help_hdl, "do help");
    qcmd_init(&_clear, "clear", _clear_hdl, "clear window");

    qcmd_add(&_help);
    qcmd_add(&_history);
    qcmd_add(&_clear);
    QSH_CLEAR_DISP;
    QPRINTF(QTERM_CBL_ON);
    QPRINTF("  ___  ____  _   _   ______   __  _    _   _  ___   ___  ___\r\n");
    QPRINTF(" / _ \\/ ___|| | | | | __ ) \\ / / | |  | | | |/ _ \\ / _ \\|_ _|\r\n");
    QPRINTF("| | | \\___ \\| |_| | |  _ \\\\ V /  | |  | | | | | | | | || | |\r\n");
    QPRINTF("| |_| |___) |  _  | | |_) || |   | |__| |_| | |_| | |_|| | |\r\n");
    QPRINTF(" \\__\\_\\____/|_| |_| |____/ |_|   |_____\\___/ \\___/ \\__\\_\\___|\r\n");
                                                             
    QSH_SHOW_PERFIX;
}

int qsh_call(const char *args)
{
    iscall = 1;
    _args_c = _strlen(args);
    _memcpy(_args, args, _args_c);
    isenter = 1;
    return qsh_exec();
}

int qsh_recv(char c)
{
    static QshFunckeyStage key_state = QSH_FUNCKEY_NONE;
    if(iscall){
        return 0;
    }
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
    return c;
}

int qcmd_isarg(const char *s, const char *arg)
{
    return _strcmp(s, arg);
}

int qsh_exec()
{
    int result = 0;
    if(isenter) {
        result = _cmd_exec(_args);
        if(iscall){
            iscall = 0;
            return result;
        }
        switch(result) {
        case QCMD_EOK:
            break;
        case QCMD_MISSING:
            QPRINTF("\r\n #! cmd missing !\r\n");
            break;
        case QCMD_PARAM_OVERFLOW:
            QPRINTF("\r\n #! param overflow !\r\n");
            break;
        case QCMD_PARAM_LESS:
            QPRINTF("\r\n #! param short !\r\n");
            break;
        case QCMD_EXEC_ERR:
            QPRINTF("\r\n #! exec error !\r\n");
            break;
        case QCMD_PARAM_ERR:
            QPRINTF("\r\n #! param error !\r\n");
            break;
        default:
            break;
        }
        _reset();
        isenter = 0;
        QSH_SHOW_PERFIX;
    }
    return result;
}

int qcmd_init(QCmdObj *qcmd, const char *name, int (*handle)(int, char **), const char *usage)
{
    qcmd->name = name;
    qcmd->id = id;
    qcmd->callback = handle;
    qcmd->usage = usage;
    id++;
    return 0;
}

#ifdef QSH_USING_LIBC
int qcmd_export(const char *name, int (*handle)(int, char **), const char *usage)
{
    QCmdObj *qcmd = (QCmdObj *)malloc(sizeof(QCmdObj));
    qcmd_init((QCmdObj *)qcmd, name, handle, usage);
    qcmd_add(qcmd);
    return 0;
}
#endif

int qcmd_add(QCmdObj *qcmd)
{
    if(_cmd_isexist(qcmd) == 0) {
        _clist_insert(&clist, &qcmd->node);
        return 0;
    } else {
        return -1;
    }
}

int qcmd_del(QCmdObj *qcmd)
{
    if(_cmd_isexist(qcmd)) {
        _cqlist_remove(&qcmd->node);
        return 0;
    } else {
        return -1;
    };
}

int _hs_hdl(int argc, char **argv)
{
    QPRINTF("\r\n");
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
    QCmdObj *_cmd;
    int i, j, k = 0;
    int len;
    uint32_t num = _cmd_num();
    QPRINTF("  Number:          %d\r\n", num);
    QPRINTF("  Commands       Usage \r\n");
    QPRINTF(" ----------     -------\r\n");
    for(i = 1; i <= num; i++) {
        _cmd = _cmd_obj(i);
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
    return 0;
}
