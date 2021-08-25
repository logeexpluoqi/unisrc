/*
 * @Author: luoqi 
 * @Date: 2021-05-26 16:10:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-05-26 16:15:03
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "qsh.h"
#include "../kernel/cmd.h"
#include "../kernel/timeslice.h"

#define QSH_INPUT_LOGO    "luoqi>$ "

static char cmd_buf[CMD_MAX_LEN];
static char hs_buf[10][CMD_MAX_LEN];
static char hs_index = 0;
static QshRecvState qsh_recv_state = QSH_RECV_NOCMD;
static unsigned int recv_cnt = 0;

static CmdObj cmd_ls;
static CmdObj cmd_kill;
static CmdObj cmd_join;
static CmdObj cmd_reboot;
static CmdObj cmd_help;
static CmdObj cmd_timer;
static CmdObj cmd_hs;
static CmdObj cmd_ps;
static CmdObj cmd_clear;

static unsigned char cmd_reboot_hdl(int argc, char* argv[]);
static unsigned char cmd_help_hdl(int argc, char* argv[]);
static unsigned char cmd_ls_hdl(int argc, char* argv[]);
static unsigned char cmd_timer_hdl(int argc, char* argv[]);
static unsigned char cmd_kill_hdl(int argc, char* argv[]);
static unsigned char cmd_join_hdl(int argc, char* argv[]);
static unsigned char cmd_hs_hdl(int argc, char* argv[]);
static unsigned char cmd_ps_hdl(int argc, char* argv[]);
static unsigned char cmd_clear_hdl(int argc, char* argv[]);

void qsh_input_logo(void);
inline void qsh_cmd_reset(void);

void qsh_task_init()
{
    cmd_init(&cmd_hs, "hs", 0, cmd_hs_hdl, "list command history");
    cmd_init(&cmd_help, "help", 0, cmd_help_hdl, "list all commands");
    cmd_init(&cmd_reboot, "reboot", 0, cmd_reboot_hdl, "reboot system");
    cmd_init(&cmd_ls, "ls", 1, cmd_ls_hdl, "list objects <task/fsm/dtask>");
    cmd_init(&cmd_timer, "timer", 0, cmd_timer_hdl, "show system abs timer");
    cmd_init(&cmd_kill, "kill", 1, cmd_kill_hdl, "kill timeslice task <task id>");
    cmd_init(&cmd_join, "proc", 1, cmd_join_hdl, "join deleted task to timeslice <task id>");
    cmd_init(&cmd_ps, "ps", 0xff, cmd_ps_hdl, "list all running tasks </-d>");
    cmd_init(&cmd_clear, "clear", 0, cmd_clear_hdl, "clear window");

    cmd_add(&cmd_hs);
    cmd_add(&cmd_help);
    cmd_add(&cmd_reboot);
    cmd_add(&cmd_ls);
    cmd_add(&cmd_timer);
    cmd_add(&cmd_kill);
    cmd_add(&cmd_join);
    cmd_add(&cmd_ps);
    cmd_add(&cmd_clear);
}

void qsh_input_logo()
{
    qsh_printf("\r");
    qsh_printf(QSH_INPUT_LOGO);
}


void qsh_get_cmd(char recv_byte)
{
    if(recv_byte != '\r' && recv_byte != '\b' && recv_byte != '^' && recv_cnt  <  60)
    { // normal charactor
        cmd_buf[recv_cnt++] = recv_byte;
        qsh_printf("%c", recv_byte);
    }
    else if(recv_byte == '\b' && recv_cnt > 0)
    { // backspace charactor
        qsh_printf("\b ");
        cmd_buf[-- recv_cnt] = 0;
        qsh_input_logo();
        qsh_printf("%s", cmd_buf);
    }
    else if(recv_byte == '\r')
    { // enter key value
        if(recv_cnt != 0)
        {
            if(cmd_buf[recv_cnt - 1] == ' ')
            {
                cmd_buf[recv_cnt - 1] = 0;
            }
            qsh_recv_state = QSH_RECV_FINISHED;
            if(!(cmd_buf[0] == 'h' && cmd_buf[1] == 's'))
            {
                memcpy(hs_buf[hs_index], cmd_buf, sizeof(cmd_buf));
                if(++ hs_index == 10)
                    hs_index = 0;
            }
            qsh_printf("\r\n");
            recv_cnt = 0;
        }
        else 
        {
            qsh_printf("\r\n");
            qsh_input_logo();
        }
    }
    else if(recv_byte == '^')
    { // history recall
        memcpy(cmd_buf, hs_buf[hs_index - 1], sizeof(hs_buf[hs_index - 1]));
        if(-- hs_index < 0)
        {
            hs_index = 9;
        }
        recv_cnt = 0;
        while(cmd_buf[recv_cnt] != 0)
        {
            recv_cnt ++;
        }
        qsh_printf("\r\033[k");
        qsh_input_logo();
        qsh_printf("%s", cmd_buf);
    }
    else if(recv_byte > 60)
    {
        qsh_printf("\r\n #! Command buffer overflow !\n");
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
            qsh_printf("\r\03[k"); // clear line 
            if(hs_index > 0){
                hs_index --;
                memcpy(cmd_buf, hs_buf[hs_index], sizeof(hs_buf[hs_index]));
                qsh_input_logo();
                qsh_printf("%s\r\n", cmd_buf);
            }
            else{
                qsh_printf("\r\03[k");
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

void qsh_cmd_reset()
{
    recv_cnt = 0;
    memset(cmd_buf, 0, sizeof(cmd_buf));
}

char* qsh_cmd()
{
    if(qsh_recv_state == QSH_RECV_FINISHED)
        return cmd_buf;
    else 
        return NULL;
}

void qsh_set_recv_state(QshRecvState state)
{
    qsh_recv_state = state;
}

QshRecvState qsh_get_recv_state()
{
    return qsh_recv_state;
}

void qsh_task_exec()
{
    if(qsh_recv_state == QSH_RECV_FINISHED)
    {
        switch(cmd_exec(qsh_cmd()))
        {
        case CMD_NO_ERR: break;
        case CMD_LEN_OUT: 
        {
            qsh_printf(" #! command length exceed !\r\n");
            break;
        }
        case CMD_NUM_OUT:
        {
            qsh_printf(" #! command quantity exceed !\r\n");
            break;
        }
        case CMD_NO_CMD:
        {
            qsh_printf(" #! command not found !\r\n");
            break;
        }
        case CMD_PARAM_EXCEED:
        {
            qsh_printf(" #! parameter exceed !\r\n");
            break;
        }
        case CMD_PARAM_LESS: 
        {
            qsh_printf(" #! parameter short !\r\n");
            break;
        }
        case CMD_EXEC_ERR:
        {
            qsh_printf(" #! command execute error !\r\n");
            break;
        }
        default:
            break;
        }
        recv_cnt = 0;
        qsh_printf(QSH_INPUT_LOGO);
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
        qsh_printf(" [%d] %s\r\n", 9 - i, hs_buf[(hs_index + i) % 10]);
    }
    return 0;
}

unsigned char cmd_reboot_hdl(int argc, char* argv[])
{
    // device software reset fucntion

    return 0;
}

unsigned char cmd_timer_hdl(int argc, char* argv[])
{
    // qsh_printf("\r>> System timer count[x100us] is: %lld\r\n", sys_get_abs_time());

    return 0;
}

unsigned char cmd_clear_hdl(int argc, char* argv[])
{
    qsh_printf("\033[H\033[J");

    return 0;
}

unsigned char cmd_help_hdl(int argc, char* argv[])
{
    CmdObj* cmd;
    unsigned int i;

    unsigned int num = cmd_num(); // except cmd_list head
    qsh_printf(" CMD NUM: %d \r\n", num);
    qsh_printf(" [Commands]           [Usage]\r\n");
    for(i = num; i > 0; i--)
    {
        cmd = cmd_obj_get(i);
        qsh_printf("  %-15s :    %s\r\n", cmd->name, cmd->usage);
    }
    qsh_printf("\r\n");
    return 0;
}

unsigned char cmd_ls_hdl(int argc, char* argv[])
{
    if(strcmp(argv[1],"task") == 0)
    {
        TimesilceTaskObj* task;
        unsigned int num = timeslice_get_task_num();
        unsigned int i;

        qsh_printf(" TASK NUM: %d, TIME TICK: %dus \r\n", num, 100);
        qsh_printf(" [Task name]                [Task ID]         [Timeslice]            [Usage]\r\n");
        for(i = num; i > 0; i--)
        {
            task = timeslice_obj_get(i);
            qsh_printf("  %-20s :     %-6d       :    %-6d          :      %s\r\n", task->name, task->id, task->timeslice_len, task->usage);
        }
    }
    else if(strcmp(argv[1], "dtask") == 0)
    {
        unsigned int i;
        TimesilceTaskObj* task;
        unsigned int num = timeslice_get_del_task_num();
        qsh_printf(" DTASK NUM: %d, TIME TICK: %dus \r\n", num, 100);
        qsh_printf(" [DTask name]               [Task ID]         [Timeslice]            [Usage]\r\n");
        for(i = num; i > 0; i--)
        {
            task = timeslice_del_obj_get(i);
            qsh_printf("  %-20s :     %-6d       :    %-6d          :      %s\r\n", task->name, task->id, task->timeslice_len, task->usage);
        }
    }
    else 
    {
        qsh_printf(" #! parameter not find !");
    }
    qsh_printf("\r\n");
    return 0;
}

unsigned char cmd_kill_hdl(int argc, char* argv[])
{
    unsigned char task_find = 0;

    TimesilceTaskObj* task;

    unsigned int num = timeslice_get_task_num();
    for(unsigned int i = num; i > 0; i--)
    {
        task = timeslice_obj_get(i);
        if(atoi(argv[1]) == task->id)
        {
            task_find = 1;
            timeslice_task_del(task);
            qsh_printf(">> Task [%s] has deleted from system\r\n", task->name);
        }
    }

    if(task_find == 0)
        qsh_printf(" #! Task not found !\r\n", task->name);

    return 0;
}

unsigned char cmd_join_hdl(int argc, char* argv[])
{
    unsigned char task_find = 0;
    unsigned int i;

    TimesilceTaskObj* task;

    unsigned int num = timeslice_get_del_task_num();
    for(i = num; i > 0; i--)
    {
        task = timeslice_del_obj_get(i);
        if(atoi(argv[1]) == task->id)
        {
            task_find = 1;
            timeslice_task_add(task);
            qsh_printf(">> Task [%s] has add to system\r\n", task->name);
        }
    }

    if(task_find == 0)
        qsh_printf(" #! Task not found !\r\n", task->name);

    return 0;
}

unsigned char cmd_ps_hdl(int argc, char* argv[])
{
    unsigned int i;
    if(argc == 1)
    {
        TimesilceTaskObj* task;
        unsigned int num = timeslice_get_task_num();
        qsh_printf(" TASK NUM: %d, TIME TICK: %dus \r\n", num, 100);
        qsh_printf(" [Task name]                [Task ID]         [Timeslice]            [Usage]\r\n");
        for(i = num; i > 0; i--)
        {
            task = timeslice_obj_get(i);
            qsh_printf("  %-20s :     %-6d       :    %-6d          :      %s\r\n", task->name, task->id, task->timeslice_len, task->usage);
        }
    }
    else if(argc == 2 && strcmp(argv[1], "-d") == 0)
    {
        TimesilceTaskObj* task;
        unsigned int num = timeslice_get_del_task_num();
        qsh_printf(" DTASK NUM: %d, TIME TICK: %dus \r\n", num, 100);
        qsh_printf(" [DTask name]               [Task ID]         [Timeslice]            [Usage]\r\n");
        for(i = num; i > 0; i--)
        {
            task = timeslice_del_obj_get(i);
            qsh_printf("  %-20s :     %-6d       :    %-6d          :      %s\r\n", task->name, task->id, task->timeslice_len, task->usage);
        } 
    }
    else 
    {
        qsh_printf(" #! parameter not find !\r\n");
    }
    return 0;
}
