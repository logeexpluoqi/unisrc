/*
 * @Author: luoqi 
 * @Date: 2021-05-26 16:10:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-08-31 17:02:45
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "qsh.h"
#include "../kernel/cmd.h"

#define QSH_INPUT_LOGO    "luoqi>$ "

static char cmd_buf[CMD_MAX_LEN];
static char hs_buf[QSH_HISTORY_MAX][CMD_MAX_LEN];
static char hs_index = 0;
static QshRecvState qsh_recv_state = QSH_RECV_NOCMD;
static unsigned int cmd_buf_size = 0;

static void qsh_clear_line(void);
static inline void qsh_cmd_reset(void);
static void qsh_save_history(char* hs_cmd, int size);
static void qsh_recall_history(void);
static int qsh_recv_spec(char recv_byte);
static void qsh_recv_buf(char recv_byte);
static void qsh_recv_backspace(void);
static void qsh_recv_enter(void);
static void qsh_recv_up(void);
static void qsh_recv_down(void);
static void qsh_recv_right(void);
static void qsh_recv_left(void);

static CmdObj cmd_reboot;
static CmdObj cmd_help;
static CmdObj cmd_hs;
static CmdObj cmd_clear;

static unsigned char cmd_reboot_hdl(int argc, char* argv[]);
static unsigned char cmd_help_hdl(int argc, char* argv[]);
static unsigned char cmd_hs_hdl(int argc, char* argv[]);
static unsigned char cmd_clear_hdl(int argc, char* argv[]);



void qsh_task_init()
{
    cmd_init(&cmd_hs, "hs", 0, cmd_hs_hdl, "list command history");
    cmd_init(&cmd_help, "help", 0, cmd_help_hdl, "list all commands");
    cmd_init(&cmd_reboot, "reboot", 0, cmd_reboot_hdl, "reboot system");
    cmd_init(&cmd_clear, "clear", 0, cmd_clear_hdl, "clear window");

    cmd_add(&cmd_hs);
    cmd_add(&cmd_help);
    cmd_add(&cmd_reboot);
    cmd_add(&cmd_clear);
    qsh_input_logo();
}

void qsh_input_logo()
{
    QSH_PRINTF("\r");
    QSH_PRINTF(QSH_INPUT_LOGO);
}

void qsh_clear_line()
{
    QSH_PRINTF("\r");
    QSH_PRINTF("\x1b[K");
}

void qsh_cmd_reset()
{
    cmd_buf_size = 0;
    memset(cmd_buf, 0, sizeof(cmd_buf));
}

char* qsh_cmd()
{
    if(qsh_recv_state == QSH_RECV_FINISHED)
        return cmd_buf;
    else 
        return NULL;
}

void qsh_save_history(char* hs_cmd, int size)
{
    memcpy(hs_buf[hs_index ++], hs_cmd, size);
    if(hs_index >= QSH_HISTORY_MAX)
    {
        hs_index = 0;
    }
}

void qsh_recall_history()
{
    if(hs_index -- <= 0)
    {
        hs_index = QSH_HISTORY_MAX - 1;
    }
    memcpy(cmd_buf, hs_buf[hs_index], sizeof(hs_buf[hs_index]));
    cmd_buf_size = strlen(cmd_buf);
}

void qsh_recv_buf(char recv_byte)
{
    cmd_buf[cmd_buf_size++] = recv_byte;
    QSH_PRINTF("%c", recv_byte);
}

void qsh_recv_backspace()
{
    QSH_PRINTF("\b ");
    cmd_buf[--cmd_buf_size] = 0;
    qsh_input_logo();
    QSH_PRINTF("%s", cmd_buf);
}

void qsh_recv_enter()
{
    if(cmd_buf_size != 0){
        if(cmd_buf[cmd_buf_size - 1] == ' '){
            cmd_buf[cmd_buf_size - 1] = 0;
        }
        if(!(cmd_buf[0] == 'h' && cmd_buf[1] == 's')){
            qsh_save_history(cmd_buf, sizeof(cmd_buf));
        }
        cmd_buf_size = 0;
        QSH_PRINTF("\r\n");
        qsh_recv_state = QSH_RECV_FINISHED;
    }else {
        QSH_PRINTF("\r\n");
        qsh_input_logo();
    }
}

void qsh_recv_up()
{
    if(hs_index > 0)
    {
        qsh_clear_line();
        qsh_input_logo();
        qsh_recall_history();
        QSH_PRINTF("%s", cmd_buf);
    }
    else
    {
        qsh_clear_line();
        qsh_input_logo();
    }
}

void qsh_recv_down()
{

}

void qsh_recv_right()
{

}

void qsh_recv_left()
{

}

int qsh_recv_spec(char recv_byte)
{
    if(recv_byte == '\x1b' || recv_byte == '\x5b' 
        || recv_byte == '\x41' // up-key 
        || recv_byte == '\x42' // down-key
        || recv_byte == '\x43' // right-key
        || recv_byte == '\x44') // left-key
    {
        if(recv_byte == '\x5b' 
            || recv_byte == '\x41' 
            || recv_byte == '\x42'
            || recv_byte == '\x43'
            || recv_byte == '\x44')
        {
            if(recv_byte == '\x41') // up-key
                return 1;
            else if(recv_byte == '\x42') // down-key
                return 2;
            else if(recv_byte == '\x43') // right -key
                return 3;
            else if(recv_byte == '\x44') // left-key
                return 4;
            else 
            {
                memset(cmd_buf, 0, sizeof(cmd_buf));
                return 0;
            }
        }
        else 
            return 0;
    }
    else
        return 0;
}

void qsh_get_cmd(char recv_byte)
{
    int spec = qsh_recv_spec(recv_byte);
    if(recv_byte != '\r' && recv_byte != '\b' && spec == 0 && cmd_buf_size  <  CMD_MAX_LEN) // normal charactor
        qsh_recv_buf(recv_byte);
    else if(recv_byte == '\b' && cmd_buf_size > 0)// backspace charactor
        qsh_recv_backspace();
    else if(recv_byte == '\r') // enter key value
        qsh_recv_enter();
    else if(spec == 1)
        qsh_recv_up();
    else if(spec == 2)
        qsh_recv_down();
    else if(spec == 3)
        qsh_recv_right();
    else if(spec == 4)
        qsh_recv_left();
    else if(cmd_buf_size > CMD_MAX_LEN)
    {
        QSH_PRINTF("\r\n>> #! Command buffer overflow !\r\n");
        qsh_input_logo();
        qsh_cmd_reset();
    }
    else
    {
        qsh_input_logo();
        qsh_cmd_reset();
    }
}

void qsh_gets_cmd(char* cmd)
{
    uint32_t len = strlen(cmd);
    if(len <= CMD_MAX_LEN && len > 0){
        if(len == 3 && cmd[0] == 0x1b && cmd[1] == 0x5b && cmd[2] == 0x41){
            qsh_clear_line(); 
            if(hs_index > 0){
                hs_index --;
                memcpy(cmd_buf, hs_buf[hs_index], sizeof(hs_buf[hs_index]));
                qsh_input_logo();
                QSH_PRINTF("%s\r\n", cmd_buf);
            }
            else{
                qsh_clear_line();
                qsh_input_logo();
                return;
            }
        }else{
            memcpy(cmd_buf, cmd, len);
        }
        if(strcmp(cmd_buf, "hs") != 0){
            memcpy(hs_buf[hs_index++], cmd_buf, sizeof(cmd_buf));
            if(hs_index == 10){
                hs_index = 0;
            }
        }
        qsh_recv_state = QSH_RECV_FINISHED;
    }else{
        qsh_input_logo();
        return;
    }
}

void qsh_task_exec()
{
    if(qsh_recv_state == QSH_RECV_FINISHED){
        switch(cmd_exec(qsh_cmd())){
        case CMD_NO_ERR: break;
        case CMD_LEN_OUT: {
            QSH_PRINTF(" #! command length exceed !\r\n");
            break;
        }
        case CMD_NUM_OUT:{
            QSH_PRINTF(" #! command quantity exceed !\r\n");
            break;
        }
        case CMD_NO_CMD:{
            QSH_PRINTF(" #! command not found !\r\n");
            break;
        }
        case CMD_PARAM_EXCEED:{
            QSH_PRINTF(" #! parameter exceed !\r\n");
            break;
        }
        case CMD_PARAM_LESS: {
            QSH_PRINTF(" #! parameter short !\r\n");
            break;
        }
        case CMD_EXEC_ERR:{
            QSH_PRINTF(" #! command execute error !\r\n");
            break;
        }
        default:
            break;
        }
        cmd_buf_size = 0;
        QSH_PRINTF(QSH_INPUT_LOGO);
        memset(cmd_buf, 0, sizeof(cmd_buf));
        qsh_cmd_reset();
        qsh_recv_state = QSH_RECV_NOCMD;
    }
}

unsigned char cmd_hs_hdl(int argc, char* argv[])
{
    int i;

    for(i = 0; i < 10; i++)
    {
        QSH_PRINTF(" [%d] %s\r\n", 9 - i, hs_buf[(hs_index + i) % 10]);
    }
    return 0;
}

unsigned char cmd_reboot_hdl(int argc, char* argv[])
{
    // device software reset fucntion

    return 0;
}

unsigned char cmd_clear_hdl(int argc, char* argv[])
{
    QSH_PRINTF("\033[H\033[J");

    return 0;
}

unsigned char cmd_help_hdl(int argc, char* argv[])
{
    CmdObj* cmd;
    unsigned int i;

    unsigned int num = cmd_num(); // except cmd_list head
    QSH_PRINTF(" CMD NUM: %d \r\n", num);
    QSH_PRINTF(" [Commands]           [Usage]\r\n");
    for(i = num; i > 0; i--)
    {
        cmd = cmd_obj_get(i);
        QSH_PRINTF("  %-15s :    %s\r\n", cmd->name, cmd->usage);
    }
    QSH_PRINTF("\r\n");
    return 0;
}
