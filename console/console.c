/*
 * @Author: luoqi 
 * @Date: 2021-05-26 16:10:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-05-26 16:15:03
 */

#include "console.h"
#include "../cmd/cmd.h"
#include "../timeslice/timeslice.h"
#include <string.h>

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

static unsigned char dbg_reboot_hdl(int argc, char* argv[]);
static unsigned char dbg_help_hdl(int argc, char* argv[]);
static unsigned char dbg_ls_hdl(int argc, char* argv[]);
static unsigned char dbg_timer_hdl(int argc, char* argv[]);
static unsigned char dbg_kill_hdl(int argc, char* argv[]);
static unsigned char dbg_proc_hdl(int argc, char* argv[]);
static unsigned char dbg_hs_hdl(int argc, char* argv[]);
static unsigned char cmd_ps_hdl(int argc, char* argv[]);

DbgErrType dbg_task_init()
{
    cmd_init(&cmd_hs, "hs", 0, dbg_hs_hdl, "list command history");
    cmd_init(&cmd_help, "help", 0, dbg_help_hdl, "list all commands");
    cmd_init(&cmd_reboot, "reboot", 0, dbg_reboot_hdl, "reboot system");
    cmd_init(&cmd_ls, "ls", 1, dbg_ls_hdl, "list objects <task/fsm/dtask>");
    cmd_init(&cmd_timer, "timer", 0, dbg_timer_hdl, "show system abs timer");
    cmd_init(&cmd_kill, "kill", 1, dbg_kill_hdl, "kill timeslice task <task id>");
    cmd_init(&cmd_proc, "proc", 1, dbg_proc_hdl, "recover deleted task <task id>");
    cmd_init(&cmd_ps, "ps", 0xff, cmd_ps_hdl, "list all running tasks </-d>");

    cmd_add(&cmd_hs);
    cmd_add(&cmd_help);
    cmd_add(&cmd_reboot);
    cmd_add(&cmd_ls);
    cmd_add(&cmd_timer);
    cmd_add(&cmd_kill);
    cmd_add(&cmd_proc);
    cmd_add(&cmd_ps);

    return DBG_NO_ERR;
}

void dbg_cmd_console(char recv_byte)
{
    if(recv_byte != '\r' && recv_byte != '\b' && recv_byte != '^' && recv_cnt  <  60)
    { // normal charactor
        cmd_buf[recv_cnt++] = recv_byte;
        sys_printf(DBG_PORT, "%c", recv_byte);
    }
    else if(recv_byte == '\b' && recv_cnt > 0)
    { // backspace charactor
        for(char i = 0; i < recv_cnt; i++)
        {
            sys_printf(DBG_PORT, " "); 
        }
        cmd_buf[-- recv_cnt] = 0;
        sys_printf(DBG_PORT, "\runicontroller$ ");
        sys_printf(DBG_PORT, "%s", cmd_buf);
    }
    else if(recv_byte == '\r')
    { // enter key value
        if(recv_cnt != 0)
        {
            dbg_set_cmd_recv_state(FINISHED);
            if(!(cmd_buf[0] == 'h' && cmd_buf[1] == 's'))
            {
                memcpy(hs_buf[hs_pos], cmd_buf, sizeof(cmd_buf));
                if(++ hs_pos == 10)
                    hs_pos = 0;
            }
            sys_printf(DBG_PORT, "\r\n");
            recv_cnt = 0;
        }
        else 
            sys_printf(DBG_PORT, "\r\nunicontroller$ ");
    }
    else if(recv_byte == '^')
    { // history recall
        memcpy(cmd_buf, hs_buf[hs_pos - 1], sizeof(hs_buf[hs_pos - 1]));
        if(-- hs_pos < 0)
            hs_pos = 9;
        recv_cnt = 0;
        while(cmd_buf[recv_cnt] != 0)
            recv_cnt ++;
        sys_printf(DBG_PORT, "\r                                                            ");
        sys_printf(DBG_PORT, "\runicontroller$ ");
        sys_printf(DBG_PORT, "%s", cmd_buf);
    }
    else if(recv_byte > 60)
    {
        sys_printf(DBG_PORT, "\r\n>> Err: Command buffer overflow !\r\nunicontroller$ ");
        dbg_cmd_reset();
        recv_cnt = 0;
    }
    else 
    {
        sys_printf(DBG_PORT, "\runicontroller$  ");
        sys_printf(DBG_PORT, "\runicontroller$ ");
        dbg_cmd_reset();
        recv_cnt = 0; 
    }
}

void dbg_cmd_reset()
{
    recv_cnt = 0;
    memset(cmd_buf, 0, sizeof(cmd_buf));
}

char* dbg_cmd()
{
    if(cmd_recv_state == FINISHED)
        return cmd_buf;
    else 
        return NULL;
}

void dbg_set_cmd_recv_state(CmdRecvState state)
{
    cmd_recv_state = state;
}

CmdRecvState dbg_get_cmd_recv_state()
{
    return cmd_recv_state;
}

DbgErrType dbg_task_exec()
{
    if(cmd_recv_state == FINISHED)
    {
        switch(cmd_exec(dbg_cmd()))
        {
        case CMD_NO_ERR: break;
        case CMD_LEN_OUT: 
        {
            sys_printf(DBG_PORT, ">> Err: command length exceed !\r\n");
            break;
        }
        case CMD_NUM_OUT:
        {
            sys_printf(DBG_PORT, ">> Err: command quantity exceed !\r\n");
            break;
        }
        case CMD_NO_CMD:
        {
            sys_printf(DBG_PORT, ">> Err: command no found !\r\n");
            break;
        }
        case CMD_PARAM_EXCEED:
        {
            sys_printf(DBG_PORT, ">> Err: parameter exceed !\r\n");
            break;
        }
        case CMD_PARAM_LESS: 
        {
            sys_printf(DBG_PORT, ">> Err: parameter short !\r\n");
            break;
        }
        case CMD_EXEC_ERR:
        {
            sys_printf(DBG_PORT, ">> Err: command execute error !\r\n");
            break;
        }
        default:
            break;
        }
        recv_cnt = 0;
        sys_printf(DBG_PORT, "unicontroller$ ");
        memset(cmd_buf, 0, sizeof(cmd_buf));
        dbg_cmd_reset();
        cmd_recv_state = NOCMD;
    }
    return DBG_NO_ERR;
}

unsigned char dbg_hs_hdl(int argc, char* argv[])
{
    for(int i = 0; i < 10; i++)
    {
        sys_printf(DBG_PORT, " [%d] %s\r\n", 9 - i, hs_buf[(hs_pos + i) % 10]);
    }
    return 0;
}

unsigned char dbg_reboot_hdl(int argc, char* argv[])
{
    SysCtl_resetDevice();
    return 0;
}

unsigned char dbg_timer_hdl(int argc, char* argv[])
{
    sys_printf(DBG_PORT, ">> System timer count[x100us] is: %lld\r\n", sys_get_abs_time());

    return 0;
}

unsigned char dbg_help_hdl(int argc, char* argv[])
{
    CmdObj* cmd;

    unsigned int num = cmd_num(); // except cmd_list head
    sys_printf(DBG_PORT, " CMD NUM: %d \r\n", num);
    sys_printf(DBG_PORT, " [Commands]           [Usage]\r\n");
    for(unsigned int i = num; i > 0; i--)
    {
        cmd = cmd_obj_get(i);
        sys_printf(DBG_PORT, "  %-15s :    %s\r\n", cmd->name, cmd->usage);
    }
    sys_printf(DBG_PORT, "\r\n");
    return 0;
}

unsigned char dbg_ls_hdl(int argc, char* argv[])
{
    if(strcmp(argv[1],"task") == 0)
    {
        TimesilceTaskObj* task;
        unsigned int num = timeslice_get_task_num();
        sys_printf(DBG_PORT, " TASK NUM: %d, TIME TICK: %dus \r\n", num, 100);
        sys_printf(DBG_PORT, " [Task name]                [Task ID]         [Timeslice]            [Usage]\r\n");
        for(unsigned int i = num; i > 0; i--)
        {
            task = timeslice_obj_get(i);
            sys_printf(DBG_PORT, "  %-20s :     %-6d       :    %-6d          :      %s\r\n", task->name, task->id, task->timeslice_len, task->usage);
        }
    }
    else if(strcmp(argv[1], "dtask") == 0)
    {
        TimesilceTaskObj* task;
        unsigned int num = timeslice_get_del_task_num();
        sys_printf(DBG_PORT, " DTASK NUM: %d, TIME TICK: %dus \r\n", num, 100);
        sys_printf(DBG_PORT, " [DTask name]               [Task ID]         [Timeslice]            [Usage]\r\n");
        for(unsigned int i = num; i > 0; i--)
        {
            task = timeslice_del_obj_get(i);
            sys_printf(DBG_PORT, "  %-20s :     %-6d       :    %-6d          :      %s\r\n", task->name, task->id, task->timeslice_len, task->usage);
        }
    }
    else 
    {
        sys_printf(DBG_PORT, ">> Err: parameter not find !");
    }
    sys_printf(DBG_PORT, "\r\n");
    return 0;
}

unsigned char dbg_kill_hdl(int argc, char* argv[])
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
            sys_printf(DBG_PORT, ">> Task [%s] has deleted from system\r\n", task->name);
        }
    }

    if(task_find == 0)
        sys_printf(DBG_PORT, ">> Task not found !\r\n", task->name);

    return 0;
}

unsigned char dbg_proc_hdl(int argc, char* argv[])
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
            sys_printf(DBG_PORT, ">> Task [%s] has add to system\r\n", task->name);
        }
    }

    if(task_find == 0)
        sys_printf(DBG_PORT, ">> Task not found !\r\n", task->name);

    return 0;
}

unsigned char cmd_ps_hdl(int argc, char* argv[])
{
    if(argc == 1)
    {
        TimesilceTaskObj* task;
        unsigned int num = timeslice_get_task_num();
        sys_printf(DBG_PORT, " TASK NUM: %d, TIME TICK: %dus \r\n", num, 100);
        sys_printf(DBG_PORT, " [Task name]                [Task ID]         [Timeslice]            [Usage]\r\n");
        for(unsigned int i = num; i > 0; i--)
        {
            task = timeslice_obj_get(i);
            sys_printf(DBG_PORT, "  %-20s :     %-6d       :    %-6d          :      %s\r\n", task->name, task->id, task->timeslice_len, task->usage);
        }
    }
    else if(argc == 2 && strcmp(argv[1], "-d") == 0)
    {
        TimesilceTaskObj* task;
        unsigned int num = timeslice_get_del_task_num();
        sys_printf(DBG_PORT, " DTASK NUM: %d, TIME TICK: %dus \r\n", num, 100);
        sys_printf(DBG_PORT, " [DTask name]               [Task ID]         [Timeslice]            [Usage]\r\n");
        for(unsigned int i = num; i > 0; i--)
        {
            task = timeslice_del_obj_get(i);
            sys_printf(DBG_PORT, "  %-20s :     %-6d       :    %-6d          :      %s\r\n", task->name, task->id, task->timeslice_len, task->usage);
        } 
    }
    else 
    {
        sys_printf(DBG_PORT, ">> Err: parameter not find !\r\n");
    }
    return 0;
}
