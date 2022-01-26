/*
 * @Author: luoqi 
 * @Date: 2021-04-27 19:19:44 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 17:03:39
 */

#ifndef _PID_H
#define _PID_H

typedef struct pid_structure {
    float kp, ki, kd;
    float kp_max, kp_min;
    float ki_max, ki_min;
    float kd_max, kd_min;
    float err_k, err_k1, err_k2;
    float u_k, u_k1;
    float delta_u_k;
} PidObj;

typedef enum {
    KP,
    KI,
    KD
} PidParam;

void pid_init(PidObj* pid, float kp, float ki, float kd);

void pid_param_set(PidObj* pid, float kp, float ki, float kd);

void pid_param_get(PidObj* pid, float* kp, float* ki, float* kd);

void pid_param_limit_set(PidObj* pid, PidParam param, float min, float max);

void pid_param_limit_get(PidObj* pid, PidParam param, float* min, float* max);

float pid_control_once(PidObj* pid, float err);

float pid_ki_separation_control_once(PidObj* pid, float err, float seplimit);

void pid_uk_clear(PidObj* pid);

#endif
