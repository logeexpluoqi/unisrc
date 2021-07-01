/*
 * @Author: luoqi 
 * @Date: 2021-05-26 16:10:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-05-26 16:15:03
 */

#include "console.h"
#include "../kernel/cmd.h"
#include "../kernel/timeslice.h"
#include <string.h>

#define CMD_USR_HEAD    ">$ "

static char cmd_buf[CMD_MAX_LEN];
static char hs_buf[10][CMD_MAX_LEN];
static char hs_pos = 0;
static CmdRecvState cmd_recv_state = NOCMD;
static unsigned int recv_cnt = 0;

static CmdObj cmd_ls;
static CmdObj cmd_kill;
static CmdObj cmd_proc;
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
static unsigned char cmd_proc_hdl(int argc, char* argv[]);
static unsigned char cmd_hs_hdl(int argc, char* argv[]);
static unsigned char cmd_ps_hdl(int argc, char* argv[]);
static unsigned char cmd_clear_hdl(int argc, char* argv[]);

inline void console_print_usr_head(void);
inline void console_set_recv_state(CmdRecvState state);
inline void console_cmd_reset(void);
inline CmdRecvState console_get_recv_state(void);

DbgErrType console_task_init()
{
    cmd_init(&cmd_hs, "hs", 0, cmd_hs_hdl, "list command history");
    cmd_init(&cmd_help, "help", 0, cmd_help_hdl, "list all commands");
    cmd_init(&cmd_reboot, "reboot", 0, cmd_reboot_hdl, "reboot system");
    cmd_init(&cmd_ls, "ls", 1, cmd_ls_hdl, "list objects <task/fsm/dtask>");
    cmd_init(&cmd_timer, "timer", 0, cmd_timer_hdl, "show system abs timer");
    cmd_init(&cmd_kill, "kill", 1, cmd_kill_hdl, "kill timeslice task <task id>");
    cmd_init(&cmd_proc, "proc", 1, cmd_proc_hdl, "recover deleted task <task id>");
    cmd_init(&cmd_ps, "ps", 0xff, cmd_ps_hdl, "list all running tasks </-d>");
    cmd_init(&cmd_clear, "clear", 0, cmd_clear_hdl, "clear window");

    cmd_add(&cmd_hs);
    cmd_add(&cmd_help);
    cmd_add(&cmd_reboot);
    cmd_add(&cmd_ls);
    cmd_add(&cmd_timer);
    cmd_add(&cmd_kill);
    cmd_add(&cmd_proc);
    cmd_add(&cmd_ps);
    cmd_add(&cmd_clear);

    return DBG_NO_ERR;
}

void console_print_usr_head()
{
    kprintf("\r");
    kprintf(CMD_USR_HEAD);
}


void console_cmd_recv(char recv_byte)
{
    if(recv_byte != '\r' && recv_byte != '\b' && recv_byte != '^' && recv_cnt  <  60)
    { // normal charactor
        cmd_buf[recv_cnt++] = recv_byte;
        kprintf("%c", recv_byte);
    }
    else if(recv_byte == '\b' && recv_cnt > 0)
    { // backspace charactor
        console_print_usr_head();
        for(char i = 0; i < recv_cnt; i++)
        {
            kprintf(" "); 
        }
        cmd_buf[-- recv_cnt] = 0;
        console_print_usr_head();
        kprintf("%s", cmd_buf);
    }
    else if(recv_byte == '\r')
    { // enter key value
        if(recv_cnt != 0)
        {
            console_set_cmd_recv_state(FINISHED);
            if(!(cmd_buf[0] == 'h' && cmd_buf[1] == 's'))
            {
                memcpy(hs_buf[hs_pos], cmd_buf, sizeof(cmd_buf));
                if(++ hs_pos == 10)
                    hs_pos = 0;
            }
            kprintf("\r\n");
            recv_cnt = 0;
        }
        else 
            kprintf("\r\n");
            console_print_usr_head();
    }
    else if(recv_byte == '^')
    { // history recall
        memcpy(cmd_buf, hs_buf[hs_pos - 1], sizeof(hs_buf[hs_pos - 1]));
        if(-- hs_pos < 0)
            hs_pos = 9;
        recv_cnt = 0;
        while(cmd_buf[recv_cnt] != 0)
            recv_cnt ++;
        kprintf("\r                                                            ");
        console_print_usr_head();
        kprintf("%s", cmd_buf);
    }
    else if(recv_byte > 60)
    {
        kprintf("\r\n>> Err: Command buffer overflow !\n");
        console_print_usr_head();
        console_cmd_reset();
        recv_cnt = 0;
    }
    else 
    {
        console_print_usr_head();
        console_cmd_reset();
        recv_cnt = 0; 
    }
}

void console_cmd_reset()
{
    recv_cnt = 0;
    memset(cmd_buf, 0, sizeof(cmd_buf));
}

char* console_cmd()
{
    if(cmd_recv_state == FINISHED)
        return cmd_buf;
    else 
        return NULL;
}

void console_set_recv_state(CmdRecvState state)
{
    cmd_recv_state = state;
}

CmdRecvState console_get_recv_state()
{
    return cmd_recv_state;
}

DbgErrType console_task_exec()
{
    if(cmd_recv_state == FINISHED)
    {
        switch(cmd_exec(console_cmd()))
        {
        case CMD_NO_ERR: break;
        case CMD_LEN_OUT: 
        {
            kprintf("\r>> Err: command length exceed !\r\n");
            break;
        }
        case CMD_NUM_OUT:
        {
            kprintf("\r>> Err: command quantity exceed !\r\n");
            break;
        }
        case CMD_NO_CMD:
        {
            kprintf("\r>> Err: command no found !\r\n");
            break;
        }
        case CMD_PARAM_EXCEED:
        {
            kprintf("\r>> Err: parameter exceed !\r\n");
            break;
        }
        case CMD_PARAM_LESS: 
        {
            kprintf("\r>> Err: parameter short !\r\n");
            break;
        }
        case CMD_EXEC_ERR:
        {
            kprintf("\r>> Err: command execute error !\r\n");
            break;
        }
        default:
            break;
        }
        recv_cnt = 0;
        kprintf(CMD_USR_HEAD);
        memset(cmd_buf, 0, sizeof(cmd_buf));
        console_cmd_reset();
        cmd_recv_state = NOCMD;
    }
    return DBG_NO_ERR;
}

unsigned char cmd_hs_hdl(int argc, char* argv[])
{
    for(int i = 0; i < 10; i++)
    {
        kprintf("\r [%d] %s\r\n", 9 - i, hs_buf[(hs_pos + i) % 10]);
    }
    return 0;
}

unsigned char cmd_reboot_hdl(int argc, char* argv[])
{
    SysCtl_resetDevice();
    return 0;
}

unsigned char cmd_timer_hdl(int argc, char* argv[])
{
    kprintf("\r>> System timer count[x100us] is: %lld\r\n", sys_get_abs_time());

    return 0;
}

unsigned char cmd_clear_hdl(int argc, char* argv[])
{
    kprintf(DBG_PORT, "\033[H\033[J");

    return 0;
}

unsigned char cmd_help_hdl(int argc, char* argv[])
{
    CmdObj* cmd;

    unsigned int num = cmd_num(); // except cmd_list head
    kprintf("\r CMD NUM: %d \r\n", num);
    kprintf(" [Commands]           [Usage]\r\n");
    for(unsigned int i = num; i > 0; i--)
    {
        cmd = cmd_obj_get(i);
        kprintf("  %-15s :    %s\r\n", cmd->name, cmd->usage);
    }
    kprintf("\r\n");
    return 0;
}

unsigned char cmd_ls_hdl(int argc, char* argv[])
{
    if(strcmp(argv[1],"task") == 0)
    {
        TimesilceTaskObj* task;
        unsigned int num = timeslice_get_task_num();
        kprintf("\r TASK NUM: %d, TIME TICK: %dus \r\n", num, 100);
        kprintf(" [Task name]                [Task ID]         [Timeslice]            [Usage]\r\n");
        for(unsigned int i = num; i > 0; i--)
        {
            task = timeslice_obj_get(i);
            kprintf("  %-20s :     %-6d       :    %-6d          :      %s\r\n", task->name, task->id, task->timeslice_len, task->usage);
        }
    }
    else if(strcmp(argv[1], "dtask") == 0)
    {
        TimesilceTaskObj* task;
        unsigned int num = timeslice_get_del_task_num();
        kprintf("\r DTASK NUM: %d, TIME TICK: %dus \r\n", num, 100);
        kprintf(" [DTask name]               [Task ID]         [Timeslice]            [Usage]\r\n");
        for(unsigned int i = num; i > 0; i--)
        {
            task = timeslice_del_obj_get(i);
            kprintf("  %-20s :     %-6d       :    %-6d          :      %s\r\n", task->name, task->id, task->timeslice_len, task->usage);
        }
    }
    else 
    {
        kprintf("\r>> Err: parameter not find !");
    }
    kprintf("\r\n");
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
            kprintf("\r>> Task [%s] has deleted from system\r\n", task->name);
        }
    }

    if(task_find == 0)
        kprintf("\r>> Task not found !\r\n", task->name);

    return 0;
}

unsigned char cmd_proc_hdl(int argc, char* argv[])
{
    unsigned char task_find = 0;

    TimesilceTaskObj* task;

    unsigned int num = timeslice_get_del_task_num();
    for(unsigned int i = num; i > 0; i--)
    {
        task = timeslice_del_obj_get(i);
        if(atoi(argv[1]) == task->id)
        {
            task_find = 1;
            timeslice_task_add(task);
            kprintf("\r>> Task [%s] has add to system\r\n", task->name);
        }
    }

    if(task_find == 0)
        kprintf("\r>> Task not found !\r\n", task->name);

    return 0;
}

unsigned char cmd_ps_hdl(int argc, char* argv[])
{
    if(argc == 1)
    {
        TimesilceTaskObj* task;
        unsigned int num = timeslice_get_task_num();
        kprintf("\r TASK NUM: %d, TIME TICK: %dus \r\n", num, 100);
        kprintf(" [Task name]                [Task ID]         [Timeslice]            [Usage]\r\n");
        for(unsigned int i = num; i > 0; i--)
        {
            task = timeslice_obj_get(i);
            kprintf("  %-20s :     %-6d       :    %-6d          :      %s\r\n", task->name, task->id, task->timeslice_len, task->usage);
        }
    }
    else if(argc == 2 && strcmp(argv[1], "-d") == 0)
    {
        TimesilceTaskObj* task;
        unsigned int num = timeslice_get_del_task_num();
        kprintf("\r DTASK NUM: %d, TIME TICK: %dus \r\n", num, 100);
        kprintf(" [DTask name]               [Task ID]         [Timeslice]            [Usage]\r\n");
        for(unsigned int i = num; i > 0; i--)
        {
            task = timeslice_del_obj_get(i);
            kprintf("  %-20s :     %-6d       :    %-6d          :      %s\r\n", task->name, task->id, task->timeslice_len, task->usage);
        } 
    }
    else 
    {
        kprintf("\r>> Err: parameter not find !\r\n");
    }
    return 0;
}
