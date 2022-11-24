/*
 * @Author: luoqi 
 * @Date: 2021-05-26 16:10:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-25 00:38:27
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "qsh.h"

typedef enum
{
    RECV_SPEC,
    RECV_UNFINISH,
    RECV_NOCMD,
    RECV_FINISHED
} RecvState;

#define QSH_USAGE_DISP_MAX  80

#define QLOGO      "/>$ "

#define QPRINTF(...)     printf(__VA_ARGS__);

#define QSH_BACKSPACE   '\b'
#define QSH_ENTER       '\r'
#define QSH_SPACE       '\20'

static char cmd_buf[CMD_MAX_LEN];
static char hs_buf[QSH_HISTORY_MAX][CMD_MAX_LEN];
static unsigned int hs_index = 0;
static unsigned int hs_num = 0;
static unsigned int recall_pos = 0;
static unsigned int recall_times = 0;
static unsigned int recall_status = 0;
static unsigned int recv_size = 0;
static unsigned int cmd_index = 0;
static RecvState recv_state = RECV_NOCMD;

static inline void clear_line(void);
static inline void cmd_reset(void);
static inline void qlogo(void);
static inline void save_history(char* cmd, int size);
static inline void recall_prev_history(void);
static inline void recall_next_history(void);
static inline void recv_buf(char recv);
static inline void recv_backspace(void);
static inline void recv_enter(void);
static inline void recv_up(void);
static inline void recv_down(void);
static inline void recv_right(void);
static inline void recv_left(void);
static int recv_spec(char recv);

static CmdObj cmd_hs;
static CmdObj cmd_clear;
static CmdObj cmd_help;
static CmdObj cmd_help_alis;

static int cmd_help_hdl(int argc, char **argv);
static int cmd_hs_hdl(int argc, char **argv);
static int cmd_clear_hdl(int argc, char **argv);
static int cmd_ls_hdl(int argc, char **argv);

void qsh_init()
{
    memset(cmd_buf, 0, sizeof(cmd_buf));
    memset(hs_buf, 0, sizeof(hs_buf));
    hs_index = 0;
    hs_num = 0;
    recall_pos = 0;
    recall_times = 0;
    recall_status = 0;
    recv_state = RECV_NOCMD;
    recv_size = 0;
    cmd_index = 0;

    cmd_init(&cmd_hs, "hs", 0, cmd_hs_hdl, "list command history");
    cmd_init(&cmd_help, "help", 0, cmd_help_hdl, "list all commands");
    cmd_init(&cmd_help_alis, "?", 0, cmd_help_hdl, "do help");
    cmd_init(&cmd_clear, "clear", 0, cmd_clear_hdl, "clear window");

    cmd_add(&cmd_help_alis);
    cmd_add(&cmd_help);
    cmd_add(&cmd_hs);
    cmd_add(&cmd_clear);
    QPRINTF("\033[H\033[J");
    QPRINTF("========== QSH BY LUOQI ==========\r\n");
    qlogo();
}

void qlogo()
{
    QPRINTF("\r");
    QPRINTF(QLOGO);
}

void clear_line()
{
    QPRINTF("\r");
    QPRINTF("\x1b[K");
}

void cmd_reset()
{
    recv_size = 0;
    memset(cmd_buf, 0, sizeof(cmd_buf));
}

char *args()
{
    if(recv_state == RECV_FINISHED){
        return cmd_buf;
    }else {
        return NULL;
    }
}

void save_history(char *cmd, int size)
{
    memcpy(hs_buf[hs_index], cmd, size);
    hs_index = (hs_index + 1) % QSH_HISTORY_MAX;
    hs_num = (hs_num + 1 > QSH_HISTORY_MAX) ? QSH_HISTORY_MAX : (hs_num + 1);
}

void recv_enter()
{
    int i = 1, h_index;
    if(recv_size != 0) {
        for( ; cmd_buf[recv_size - i] == QSH_SPACE; i++) {
            cmd_buf[recv_size - i] = 0;
        }
        
        if(strcmp(cmd_buf, "hs") != 0) { 
            h_index = (hs_index + QSH_HISTORY_MAX - 1) % QSH_HISTORY_MAX;
            if(strcmp(cmd_buf, hs_buf[h_index]) != 0) {
                save_history(cmd_buf, sizeof(cmd_buf));
            }
        }
        
        if(recall_status == -1) {
            if(recall_times != hs_num) {
                recall_times --;
            } else {
                recall_times = hs_num;
            }
        } else if(recall_status == 1) {
            if(recall_times != 0) {
                recall_times ++;
            } else {
                recall_times = 0;
            }
        }

        recall_pos = hs_index;
        recall_times = 0;
        recv_size = 0;
        cmd_index = 0;
        QPRINTF("\r\n");
        recv_state = RECV_FINISHED;
    } else {
        QPRINTF("\r\n");
        qlogo();
    }
    recall_status = 0;
}

void recall_prev_history()
{
    recall_status = 1;
    if(recall_times  < hs_num) {
        recall_pos  = (recall_pos - 1 + QSH_HISTORY_MAX) % QSH_HISTORY_MAX;
        recall_times ++;
    } else {
        recall_times = hs_num;
    }
    memcpy(cmd_buf, hs_buf[recall_pos], sizeof(hs_buf[recall_pos]));
    recv_size = strlen(cmd_buf);
}

void recall_next_history()
{
    recall_status = -1;
    if(recall_times > 0) {
        recall_pos = (recall_pos + 1) % QSH_HISTORY_MAX;
        memcpy(cmd_buf, hs_buf[recall_pos], sizeof(hs_buf[recall_pos]));
        recv_size = strlen(cmd_buf);
        recall_times --;
    } else {
        recall_times = 0;
        cmd_reset();
    }
}

void recv_buf(char recv)
{
    cmd_buf[recv_size++] = recv;
    QPRINTF("%c", recv);
}

void recv_backspace()
{
    QPRINTF("\b ");
    cmd_buf[--recv_size] = 0;
    qlogo();
    QPRINTF("%s", cmd_buf);
}

void recv_up()
{
    if(hs_num > 0) {
        clear_line();
        qlogo();
        recall_prev_history();
        QPRINTF("%s", cmd_buf);
    } else {
        clear_line();
        qlogo();
    }
}

void recv_down()
{
    if(hs_num > 0) {
        clear_line();
        qlogo();
        recall_next_history();
        QPRINTF("%s", cmd_buf);
    } else {
        clear_line();
        qlogo();
    }
}

void recv_right()
{

}

void recv_left()
{

}

int recv_spec(char recv)
{
    if(recv == '\x1b' || recv == '\x5b' 
        || recv == '\x41'      // up-key 
        || recv == '\x42'      // down-key
        || recv == '\x43'      // right-key
        || recv == '\x44') {   // left-key
        if(recv == '\x5b' 
           || recv == '\x41' 
           || recv == '\x42'
           || recv == '\x43'
           || recv == '\x44') {
            if(recv == '\x41') {// up-key
                return 1;
            } else if(recv == '\x42') {    // down-key
                return 2;
            } else if(recv == '\x43') {    // right -key
                return 3;
            } else if(recv == '\x44') {    // left-key
                return 4;
            } else {
                memset(cmd_buf, 0, sizeof(cmd_buf));
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

void qsh_recv(char recv)
{
    int spec = 0;
    spec = recv_spec(recv);
    if((recv != QSH_ENTER) && (recv != QSH_BACKSPACE) && (spec == 0) && (recv_size  <  CMD_MAX_LEN)) {
        recv_buf(recv);
    } else if(recv == QSH_BACKSPACE && recv_size > 0) {// backspace charactor
        recv_backspace();
    } else if(recv == QSH_ENTER) {
        recv_enter();
    } else if(spec == 1) {
        recv_up();
    } else if(spec == 2) {
        recv_down();
    } else if(spec == 3) {
        recv_right();
    } else if(spec == 4) {
        recv_left();
    } else {
        if(recv_size > 0) {
            cmd_reset();
            QPRINTF("\r\n>> #! Command buffer overflow !\r\n");
        }
        qlogo();
    }
}

void qsh_exec()
{
    if(recv_state == RECV_FINISHED) {
        switch(cmd_exec(args())){
            case CMD_NO_ERR: break;
            case CMD_LEN_OUT: 
                QPRINTF(" #! command length exceed !\r\n");
                break;
            
            case CMD_NUM_OUT:
                QPRINTF(" #! command quantity exceed !\r\n");
                break;
            
            case CMD_NO_CMD:
                QPRINTF(" #! command not found !\r\n");
                break;
            
            case CMD_PARAM_EXCEED:
                QPRINTF(" #! parameter exceed !\r\n");
                break;
            
            case CMD_PARAM_LESS: 
                QPRINTF(" #! parameter short !\r\n");
                break;
            
            case CMD_EXEC_ERR:
                QPRINTF(" #! command execute error !\r\n");
                break;
                
            case CMD_PARAM_ERR:
                QPRINTF(" #! command parameter error !\r\n");
                break;

            default:
                break;
        }
        recv_size = 0;
        memset(cmd_buf, 0, sizeof(cmd_buf));
        cmd_reset();
        QPRINTF(QLOGO);
        recv_state = RECV_NOCMD;
    }
}

void qcmd_init(CmdObj* qcmd, const char* name, int (*handle)(int, char**), const char* usage)
{
    cmd_init((CmdObj*)qcmd, name, 0xff, handle, usage);
}

int qcmd_export(const char *name, int (*handle)(int, char**), const char *usage)
{
    CmdObj *qcmd = (CmdObj*)malloc(sizeof(CmdObj));
    cmd_init((CmdObj*)qcmd, name, 0xff, handle, usage);
    cmd_add(qcmd);
}

void qcmd_add(CmdObj* qcmd)
{
    cmd_add((CmdObj *)qcmd);
}

void qcmd_del(CmdObj* qcmd)
{
    cmd_del((CmdObj *)qcmd);
}

int cmd_hs_hdl(int argc, char **argv)
{
    char hs_pos;
    if((hs_index - hs_num) >= 0) {
        hs_pos  = hs_index - hs_num;
    } else {
        hs_pos = hs_index;
    }
    
    for(int i = 0; i < hs_num; i++) {
        QPRINTF(" %2d: %s\r\n", hs_num - i, hs_buf[(hs_pos + i) % QSH_HISTORY_MAX]);
    }
    return 0;
}

int cmd_clear_hdl(int argc, char **argv)
{
    QPRINTF("\033[H\033[J");

    return 0;
}

int cmd_help_hdl(int argc, char **argv)
{
    CmdObj* _cmd;
    int i, len, k = 0, j;

    unsigned int num = cmd_num();
    QPRINTF(" Commands: <%d>\r\n", num);
    QPRINTF(" [Commands]     [Usage]\r\n");
    QPRINTF(" ----------     -------\r\n");
    for(i = num; i > 0; i--) {
        _cmd = cmd_obj(i);
        QPRINTF("  -%-9s     ", _cmd->name);
        len = strlen(_cmd->usage);
        if(len < QSH_USAGE_DISP_MAX){
            QPRINTF("%s\r\n", _cmd->usage);
        }else{
            while (len > QSH_USAGE_DISP_MAX){
                k++;
                len = len - QSH_USAGE_DISP_MAX;
            }
            
            for(j = 0; j <= k; j++){
                if(j == 0){
                    QPRINTF("%-60.60s\r\n", _cmd->usage + j * QSH_USAGE_DISP_MAX);
                }else{
                    QPRINTF("                   %-80.80s\r\n", _cmd->usage + j * QSH_USAGE_DISP_MAX + 1);
                }
            }
        }
        k = 0;
    }
    return CMD_NO_ERR;
}
