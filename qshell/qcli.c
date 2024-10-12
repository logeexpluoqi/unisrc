/**
 * @ Author: luoqi
 * @ Create Time: 2024-08-01 22:16
 * @ Modified by: luoqi
 * @ Modified time: 2024-09-25 15:54
 * @ Description:
 */

#include "qcli.h"

static const char *_CLEAR_LINE = "\r\x1b[K";
static const char *_PERFIX = "\\>$ ";
static const char *_CLEAR_DISP = "\033[H\033[2J";

#define _QCLI_NULL           ((void *)0)

#define _KEY_BACKSPACE       '\b'
#define _KEY_SPACE           '\x20'
#define _KEY_ENTER           '\r'
#define _KEY_ESC             '\x1b'
#define _KEY_TAB             '\t'
#define _KEY_UP              '\x41'
#define _KEY_DOWN            '\x42'
#define _KEY_RIGHT           '\x43'
#define _KEY_LEFT            '\x44'
#define _KEY_DEL             '\x7f'

#define _QCLI_SU(n)             "\033["#n"S"   // scroll up
#define _QCLI_SD(n)             "\033["#n"T"   // scroll down
#define _QCLI_CUU(n)            "\033["#n"A"   // cursor up
#define _QCLI_CUD(n)            "\033["#n"B"   // cursor down
#define _QCLI_CUF(n)            "\033["#n"C"   // cursor front
#define _QCLI_CUB(n)            "\033["#n"D"   // cursor back
#define _QCLI_ICH(n)            "\033["#n"@"   // insert charactor
#define _QCLI_DCH(n)            "\033["#n"P"   // delete charactor
#define _QCLI_ECH(n)            "\033["#n"X"   // erase charactor
#define _QCLI_IL(n)             "\033["#n"L"   // inset line
#define _QCLI_DL(n)             "\033["#n"M"   // delete line
#define _QCLI_CBL_ON            "\033[?12h"    // cursor blink on
#define _QCLI_CBL_OFF           "\033[?12l"    // cursor blink off
#define _QCLI_CDISP_ON          "\033[1?25h"   // cursor display on
#define _QCLI_CDISP_OFF         "\033[1?25l"   // cursor display off
#define _QCLI_CSAP_USR          "\033[0SPq"    // cursor shape user
#define _QCLI_CSAP_BB           "\033[1SPq"    // cursor shape blinking block
#define _QCLI_CSAP_BBAR         "\033[5SPq"    // cursor shape blinking bar
#define _QCLI_CSAP_SBAR         "\033[6SPq"    // cursor shape steady bar

#define QCLI_ENTRY(ptr, type, member)     ((type *)((char *)(ptr) - ((unsigned long) &((type*)0)->member)))
#define QCLI_ITERATOR(node, cmds)      for (node = (cmds)->next; node != (cmds); node = node->next)

static void *_memcpy(void *dest, const void *src, uint32_t len)
{
    char *d;
    const char *s;
    if(((char *)dest > ((char *)src + len)) || ((char *)dest < (char *)src)) {
        d = dest;
        s = src;
        while(len--) {
            *d++ = *s++;
        }
    } else {
        d = (char *)((char *)dest + len - 1);
        s = (char *)((char *)src + len - 1);
        while(len--) {
            *d-- = *s--;
        }
    }
    return dest;
}

static void *_memset(void *dest, int c, uint32_t n)
{
    if((dest == _QCLI_NULL) || (n == 0)) {
        return _QCLI_NULL;
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
    if((dest == _QCLI_NULL) || (src == _QCLI_NULL)) {
        return _QCLI_NULL;
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

static inline void _list_insert(QCliList *list, QCliList *node)
{
    list->next->prev = node;
    node->next = list->next;

    list->next = node;
    node->prev = list;
}

static inline void _list_remove(QCliList *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;

    node->next = node->prev = node;
}

static int _cmd_isexist(QCliInterface *cli, QCliCmd *cmd)
{
    QCliCmd *_cmd;
    QCliList *_node;

    QCLI_ITERATOR(_node, &cli->cmds)
    {
        _cmd = QCLI_ENTRY(_node, QCliCmd, node);
        if(_strcmp(_cmd->name, cmd->name) == 0) {
            return 1;
        } else {
            continue;
        }
    }
    return 0;
}

static QCliCmd _history;
static int _history_cb(int argc, char **argv)
{
    for(uint8_t i = _history.cli->history_num; i > 0; i--) {
        _history.cli->print("%2d: %s\r\n", i, _history.cli->history[(_history.cli->history_index - i + QCLI_HISTORY_MAX) % QCLI_HISTORY_MAX]);
    }

    return 0;
}

static QCliCmd _help;
#define QCLI_USAGE_DISP_MAX 80
static int _help_cb(int argc, char **argv)
{
    QCliList *_node;
    QCliCmd *_cmd;
    uint32_t j, k = 0;
    int len;
    _help.cli->print("  Commands       Usage \r\n");
    _help.cli->print(" ----------     -------\r\n");
    QCLI_ITERATOR(_node, &_help.cli->cmds)
    {
        _cmd = QCLI_ENTRY(_node, QCliCmd, node);
        _help.cli->print(" .%-9s     - ", _cmd->name);
        len = _strlen(_cmd->usage);
        if(len < QCLI_USAGE_DISP_MAX) {
            _help.cli->print("%s\r\n", _cmd->usage);
        } else {
            while(len > QCLI_USAGE_DISP_MAX) {
                k++;
                len = len - QCLI_USAGE_DISP_MAX;
            }

            for(j = 0; j <= k; j++) {
                if(j == 0) {
                    _help.cli->print("%-80.80s\r\n", _cmd->usage + j * QCLI_USAGE_DISP_MAX);
                } else {
                    _help.cli->print("                   %-80.80s\r\n", _cmd->usage + j * QCLI_USAGE_DISP_MAX);
                }
            }
        }
        k = 0;
    }
    return 0;
}

static QCliCmd _clear;
static int _clear_cb(int argc, char **argv)
{
    _clear.cli->print(_CLEAR_DISP);

    return 0;
}

static int _parser(QCliInterface *cli, char *str, uint16_t len)
{
    if(len > QCLI_CMD_STR_MAX) {
        return -1;
    }
    for(uint16_t i = 0; i < len; i++) {
        if((str[i] != _KEY_SPACE) && (i == 0)) {
            cli->argv[cli->argc] = str;
            cli->argc++;
            continue;
        }

        if(str[i] == _KEY_SPACE) {
            str[i] = 0;
            if((str[i + 1] != _KEY_SPACE) && (str[i + 1] != '\0')) {
                cli->argv[cli->argc] = &str[i + 1];
                if(cli->argc > QCLI_CMD_ARGC_MAX) {
                    return -2;
                } else {
                    cli->argc++;
                }
            }
        }
    }
    return 0;
}

static int _cmd_callback(QCliInterface *cli)
{
    QCliList *_node;
    QCliCmd *_cmd;
    int result = 0;
    QCLI_ITERATOR(_node, &cli->cmds)
    {
        _cmd = QCLI_ENTRY(_node, QCliCmd, node);
        if(_strcmp(cli->argv[0], _cmd->name) == 0) {
            result = _cmd->callback(cli->argc, cli->argv);
            if(result == QCLI_EOK) {
                return 0;
            } else if(result == QCLI_ERR_PARAM) {
                cli->print("\r\n #! parameter error !");
            } else if(result == QCLI_ERR_PARAM_LESS) {
                cli->print("\r\n #! parameter less !");
            } else if(result == QCLI_ERR_PARAM_MORE) {
                cli->print("\r\n #! parameter more !");
            } else if(result == QCLI_ERR_PARAM_TYPE) {
                cli->print("\r\n #! parameter type error !");
            } else {
                cli->print("\r\n #! unknown error !");
            }
            return 0;
        } else {
            continue;
        }
    }
    cli->print(" #! command not found !");
    return -1;
}

int qcli_init(QCliInterface *cli, QCliPrint print)
{
    cli->cmds.next = cli->cmds.prev = &cli->cmds;
    cli->print = print;
    cli->is_exec_str = 0;
    cli->argc = 0;
    cli->args_size = 0;
    cli->args_index = 0;
    cli->history_num = 0;
    cli->history_index = 0;
    cli->history_recall_index = 0;
    cli->history_recall_times = 0;
    _memset(cli->history, 0, QCLI_HISTORY_MAX * QCLI_CMD_STR_MAX * sizeof(char));
    _memset(cli->args, 0, QCLI_CMD_STR_MAX * sizeof(char));
    _memset(&cli->argv, 0, QCLI_CMD_ARGC_MAX * sizeof(char));
    qcli_add(cli, &_help, "?", _help_cb, "help");
    qcli_add(cli, &_clear, "clear", _clear_cb, "clear screen");
    qcli_add(cli, &_history, "hs", _history_cb, "show history");
    cli->print(_CLEAR_DISP);
    cli->print("-------------------QCLI BY LUOQI-------------------\r\n");
    cli->print(_PERFIX);
    return 0;
}

int qcli_add(QCliInterface *cli, QCliCmd *cmd, const char *name, QCliCallback callback, const char *usage)
{
    cmd->name = name;
    cmd->callback = callback;
    cmd->usage = usage;
    if(_cmd_isexist(cli, cmd) == 0) {
        _list_insert(&cli->cmds, &cmd->node);
        cmd->cli = cli;
        return 0;
    } else {
        return -1;
    }
}

int qcli_remove(QCliInterface *cli, QCliCmd *cmd)
{
    if(_cmd_isexist(cli, cmd) == 0) {
        _list_remove(&cmd->node);
        return 0;
    } else {
        return -1;
    }
}

int qcli_exec(QCliInterface *cli, char c)
{
    if(c == '\x1b') {
        return 0;
    }
    if(c == '\x5b') {
        return 0;
    }
    if((c == _KEY_BACKSPACE) || (c == _KEY_DEL)) {
        if((cli->args_size > 0) && (cli->args_size == cli->args_index)) {
            cli->args_size--;
            cli->args_index--;
            cli->args[cli->args_size] = '\0';
            cli->print("\b \b");
        } else if((cli->args_size > 0) && (cli->args_size != cli->args_index) && (cli->args_index > 0)) {
            cli->args_size--;
            cli->args_index--;
            _strdelete(cli->args, cli->args_index, 1);
            cli->print(_QCLI_CUB(1));
            cli->print(_QCLI_DCH(1));
            return 0;
        } else {
            return 0;
        }
    } else if(c == _KEY_ENTER) {
        if(cli->args_size == 0) {
            if(!cli->is_exec_str){
                cli->print("\r\n%s", _PERFIX);
            }
            return 0;
        } else {
            if(!cli->is_exec_str){
                cli->print("\r\n");
            }
        }
        if((_strcmp(cli->args, "hs") != 0) && (!cli->is_exec_str)) {
            if(_strcmp(cli->history[(cli->history_index == 0) ? QCLI_HISTORY_MAX : (cli->history_index - 1) % QCLI_HISTORY_MAX], cli->args) != 0) {
                _memset(cli->history[cli->history_index], 0, _strlen(cli->history[cli->history_index]));
                _memcpy(cli->history[cli->history_index], cli->args, cli->args_size);
                cli->history_index = (cli->history_index + 1) % QCLI_HISTORY_MAX;
                if(cli->history_num < QCLI_HISTORY_MAX) {
                    cli->history_num++;
                }
            }
        }
        if(_parser(cli, cli->args, cli->args_size) != 0) {
            _memset(cli->args, 0, cli->args_size);
            _memset(&cli->argv, 0, cli->argc);
            cli->args_size = 0;
            cli->args_index = 0;
            cli->argc = 0;
            cli->history_recall_times = 0;
            cli->history_recall_index = cli->history_index;
            cli->print(" #! parse error !\r\n%s", _PERFIX);
            return 0;
        }
        _cmd_callback(cli);
        _memset(cli->args, 0, cli->args_size);
        _memset(&cli->argv, 0, cli->argc);
        cli->args_size = 0;
        cli->args_index = 0;
        cli->argc = 0;
        cli->history_recall_times = 0;
        cli->history_recall_index = cli->history_index;
        if(!cli->is_exec_str){
            cli->print("\r\n%s", _PERFIX);
        }
    } else if(c == _KEY_UP) {
        if(cli->history_num == 0) {
            cli->history_recall_times = 0;
            return 0;
        }
        if(cli->history_recall_times < cli->history_num) {
            if(cli->history_recall_index == 0) {
                cli->history_recall_index = QCLI_HISTORY_MAX;
            }
            cli->history_recall_index = (cli->history_recall_index - 1) % QCLI_HISTORY_MAX;
            _memset(cli->args, 0, cli->args_size);
            cli->args_size = _strlen(cli->history[cli->history_recall_index]);
            cli->args_index = cli->args_size;
            _memcpy(cli->args, cli->history[cli->history_recall_index], cli->args_size);
            cli->history_recall_times++;
            cli->print("%s%s%s", _CLEAR_LINE, _PERFIX, cli->args);
        } else {
            return 0;
        }
    } else if(c == _KEY_DOWN) {
        if(cli->history_num == 0) {
            return 0;
        }
        if(cli->history_recall_times > 1) {
            cli->history_recall_index = (cli->history_recall_index + 1) % QCLI_HISTORY_MAX;
            _memset(cli->args, 0, cli->args_size);
            cli->args_size = _strlen(cli->history[cli->history_recall_index]);
            cli->args_index = cli->args_size;
            _memcpy(cli->args, cli->history[cli->history_recall_index], cli->args_size);
            cli->history_recall_times--;
            cli->print("%s%s%s", _CLEAR_LINE, _PERFIX, cli->args);
        } else {
            _memset(cli->args, 0, cli->args_size);
            _memset(&cli->argv, 0, cli->argc);
            cli->args_size = 0;
            cli->args_index = 0;
            cli->argc = 0;
            cli->print("%s%s", _CLEAR_LINE, _PERFIX);
            return 0;
        }
    } else if(c == _KEY_LEFT) {
        if(cli->args_index > 0) {
            cli->print(_QCLI_CUB(1));
            cli->args_index--;
        } else {
            return 0;
        }
    } else if(c == _KEY_RIGHT) {
        if(cli->args_index < cli->args_size) {
            cli->print(_QCLI_CUF(1));
            cli->args_index++;
        } else {
            return 0;
        }
    } else if(c == _KEY_ESC) {

    } else if(c == _KEY_TAB) {

    } else {
        if(cli->args_size > QCLI_CMD_STR_MAX) {
            return 0;
        }
        if(cli->args_size == cli->args_index) {
            cli->args[cli->args_size] = c;
            cli->args_size++;
            cli->args_index = cli->args_size;
        } else {
            _strinsert(cli->args, cli->args_index, &c, 1);
            cli->args_size++;
            cli->args_index++;
            cli->print(_QCLI_ICH(1));
        }
        cli->print("%c", c);
    }
    return 0;
}

int qcli_exec_str(QCliInterface *cli, char *str)
{
    uint16_t len;
    int ret;
    if(cli == _QCLI_NULL || str == _QCLI_NULL) {
        return -1;
    }
    len = _strlen(str);
    _memset(cli->args, 0, cli->args_size);
    _memcpy(cli->args, str, len);
    cli->args_size = len;
    cli->is_exec_str = 1;
    ret = qcli_exec(cli, _KEY_ENTER);
    cli->is_exec_str = 0;
    return ret;
}
