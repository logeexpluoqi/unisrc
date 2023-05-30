/*
 * @Author: luoqi 
 * @Date: 2021-04-27 19:19:44 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 17:03:39
 */

#ifndef _PID_H
#define _PID_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct pid_structure
{
    float kp, ki, kd;
    float err_k1, err_k2;
    float u_k, u_k1;
    float delta_u_k;
    float olimit;
} PidObj;

int pid_init(PidObj *pid, float kp, float ki, float kd, float olimit);

int pid_param_set(PidObj *pid, float kp, float ki, float kd);

int pid_param_get(PidObj *pid, float *kp, float *ki, float *kd);

float pid_calcu(PidObj *pid, float err);

float pid_ki_isolate_calcu(PidObj *pid, float err, float seplimit);

int pid_clear(PidObj *pid);

int pid_calcu_clear(PidObj *pid);

#ifdef __cplusplus
}
#endif

#endif

