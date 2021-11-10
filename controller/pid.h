/*
 * @Author: luoqi 
 * @Date: 2021-04-27 19:19:44 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-04-28 18:22:00
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

void pid_init(PidObj* obj,
              float kp, float ki, float kd,
              float kp_min, float kp_max,
              float ki_min, float ki_max,
              float kd_min, float kd_max);

void pid_set_param(PidObj* obj, float kp, float ki, float kd);

void pid_get_param(PidObj* obj, float* kp, float* ki, float* kd);

void pid_set_param_limit(PidObj* obj, PidParam param, float min, float max);

void pid_get_param_limit(PidObj* obj, PidParam param, float* min, float* max);

float pid_control(PidObj* obj, float err);

float pid_control_ki_separation(PidObj* obj, float err, float seplimit);

void pid_clear_uk(PidObj* obj);

#endif
