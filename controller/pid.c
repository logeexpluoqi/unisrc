/*
 * @Author: luoqi 
 * @Date: 2021-04-27 19:20:38 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 17:05:17
 */

#include "pid.h"

void pid_init(PidObj* pid, float kp, float ki, float kd)
{
    pid->delta_u_k  = 0.0f;
    pid->err_k2     = 0.0f;
    pid->err_k1     = 0.0f;
    pid->err_k      = 0.0f;
    pid->u_k1       = 0.0f;
    pid->u_k        = 0.0f;
    pid->kp         = kp;
    pid->ki         = ki;
    pid->kd         = kd;
    pid->kp_min     = 0;
    pid->kp_max     = 3.4e38;
    pid->ki_min     = 0;
    pid->ki_max     = 3.4e38;
    pid->kd_min     = 0;
    pid->kd_max     = 3.4e38;
}

void pid_param_set(PidObj *pid, float kp, float ki, float kd)
{
    pid->kp = (pid->kp_min == 0 && pid->kp_max == 0) ? kp : \
               ((kp > pid->kp_max) ? pid->kp_max : \
               ((kp < pid->kp_min) ? pid->kp_min : kp));

    pid->ki = (pid->ki_min == 0 && pid->ki_max == 0) ? ki : \
               ((ki > pid->ki_max) ? pid->ki_max : \
               ((ki < pid->ki_min) ? pid->ki_min : ki));

    pid->kd = (pid->kd_min == 0 && pid->kd_max == 0) ? kd : \
               ((kd > pid->kd_max) ? pid->kd_max : \
               ((kd < pid->kd_min) ? pid->kd_min : kd));
}

void pid_param_get(PidObj *pid, float *kp, float *ki, float *kd)
{
    *kp = pid->kp;
    *ki = pid->ki;
    *kd = pid->kd;
}

void pid_param_limit_set(PidObj *pid, PidParam param, float min, float max)
{
    switch (param)
    {
    case KP:
        pid->kp_min = min;
        pid->kp_max = max;
        break;
    case KI:
        pid->ki_min = min;
        pid->ki_max = max;
        break;
    case KD:
        pid->kd_min = min;
        pid->kd_max = max;
        break;
    }
}

void pid_param_limit_get(PidObj *pid, PidParam param, float *min, float *max)
{
    switch (param)
    {
    case KP:
        *min = pid->kp_min;
        *max = pid->kp_max;
        break;
    case KI:
        *min = pid->ki_min;
        *max = pid->ki_max;
        break;
    case KD:
        *min = pid->kd_min;
        *max = pid->kd_max;
        break;
    }
}

float pid_control_once(PidObj *pid, float err)
{
    pid->err_k2 = pid->err_k1;
    pid->err_k1 = pid->err_k;
    pid->err_k = err;
    pid->u_k1 = pid->u_k;

    pid->delta_u_k = pid->kp * (pid->err_k - pid->err_k1) +
                     pid->ki * pid->err_k +
                     pid->kd * (pid->err_k - 2 * pid->err_k1 + pid->err_k2);

    pid->u_k = pid->u_k1 + pid->delta_u_k;
    return pid->u_k;
}

void pid_uk_clear(PidObj* pid)
{
    pid->u_k = 0;
    pid->u_k1 = 0;
    pid->delta_u_k = 0;
    pid->err_k = 0;
    pid->err_k1 = 0;
    pid->err_k2 = 0;
}

float pid_ki_separation_control_once(PidObj* pid, float err, float seplimit)
{
    unsigned char beta = err > seplimit ? 0 : 1;

    pid->err_k2 = pid->err_k1;
    pid->err_k1 = pid->err_k;
    pid->err_k = err;
    pid->u_k1 = pid->u_k;

    pid->delta_u_k = pid->kp * (pid->err_k - pid->err_k1) +
                     beta * pid->ki * pid->err_k +
                     pid->kd * (pid->err_k - 2 * pid->err_k1 + pid->err_k2);

    pid->u_k = pid->u_k1 + pid->delta_u_k;
    return pid->u_k;
}
