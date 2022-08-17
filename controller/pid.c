/*
 * @Author: luoqi 
 * @Date: 2021-04-27 19:20:38 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 17:05:17
 */

#include "pid.h"

void pid_init(PidObj* pid, float kp, float ki, float kd, float olimit)
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
    pid->olimit     = olimit;
}

void pid_param_set(PidObj *pid, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

void pid_param_get(PidObj *pid, float *kp, float *ki, float *kd)
{
    *kp = pid->kp;
    *ki = pid->ki;
    *kd = pid->kd;
}

float pid_calcu(PidObj *pid, float err)
{
    pid->err_k2 = pid->err_k1;
    pid->err_k1 = pid->err_k;
    pid->err_k = err;
    pid->u_k1 = pid->u_k;

    pid->delta_u_k = pid->kp * (pid->err_k - pid->err_k1) +
                     pid->ki * pid->err_k +
                     pid->kd * (pid->err_k - 2 * pid->err_k1 + pid->err_k2);

    pid->u_k = pid->u_k1 + pid->delta_u_k;
    if(pid->olimit == 0.0){
        return pid->u_k;
    }else{
        if(pid->u_k > pid->olimit){
            pid->u_k = pid->olimit;
            return pid->u_k;
        }else if(pid->u_k < -pid->olimit){
            pid->u_k = -pid->olimit;
            return pid->u_k;
        }else{
            return pid->u_k;
        }
    }
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

float pid_ki_isolate_calcu(PidObj* pid, float err, float seplimit)
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
    if(pid->olimit == 0.0){
        return pid->u_k;
    }else{
        if(pid->u_k > pid->olimit){
            pid->u_k = pid->olimit;
            return pid->u_k;
        }else if(pid->u_k < -pid->olimit){
            pid->u_k = -pid->olimit;
            return pid->u_k;
        }else{
            return pid->u_k;
        }
    }
}
