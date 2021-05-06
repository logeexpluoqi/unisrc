/*
 * @Author: luoqi 
 * @Date: 2021-04-27 19:20:38 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-04-28 19:17:22
 */

#include "./pid.h"

void pid_init(PidObj* obj)
{
    obj->delta_u_k  = 0.0f;
    obj->err_k2     = 0.0f;
    obj->err_k1     = 0.0f;
    obj->err_k      = 0.0f;
    obj->u_k1       = 0.0f;
    obj->u_k        = 0.0f;
    obj->kp         = 1.0f;
    obj->ki         = 0.0f;
    obj->kd         = 0.0f;
    obj->kp_min     = 0.0f;
    obj->kp_max     = 5000.0f;
    obj->ki_min     = 0.0f;
    obj->ki_max     = 1000.0f;
    obj->kd_min     = 0.0f;
    obj->kd_max     = 2000.0f;
}

void pid_set_param(PidObj *obj, float kp, float ki, float kd)
{
    obj->kp = (obj->kp_min == 0 && obj->kp_max == 0) ? kp : \
               ((kp > obj->kp_max) ? obj->kp_max : \
               ((kp < obj->kp_min) ? obj->kp_min : kp));

    obj->ki = (obj->ki_min == 0 && obj->ki_max == 0) ? ki : \
               ((ki > obj->ki_max) ? obj->ki_max : \
               ((ki < obj->ki_min) ? obj->ki_min : ki));

    obj->kd = (obj->kd_min == 0 && obj->kd_max == 0) ? kd : \
               ((kd > obj->kd_max) ? obj->kd_max : \
               ((kd < obj->kd_min) ? obj->kd_min : kd));
}

void pid_get_param(PidObj *obj, float *kp, float *ki, float *kd)
{
    *kp = obj->kp;
    *ki = obj->ki;
    *kd = obj->kd;
}

void pid_set_param_limit(PidObj *obj, PidParam param, float min, float max)
{
    switch (param)
    {
    case KP:
        obj->kp_min = min;
        obj->kp_max = max;
        break;
    case KI:
        obj->ki_min = min;
        obj->ki_max = max;
        break;
    case KD:
        obj->kd_min = min;
        obj->kd_max = max;
        break;
    }
}

void pid_get_param_limit(PidObj *obj, PidParam param, float *min, float *max)
{
    switch (param)
    {
    case KP:
        *min = obj->kp_min;
        *max = obj->kp_max;
        break;
    case KI:
        *min = obj->ki_min;
        *max = obj->ki_max;
        break;
    case KD:
        *min = obj->kd_min;
        *max = obj->kd_max;
        break;
    }
}

float pid_control(PidObj *obj, float err)
{
    obj->err_k2 = obj->err_k1;
    obj->err_k1 = obj->err_k;
    obj->err_k = err;
    obj->u_k1 = obj->u_k;

    obj->delta_u_k = obj->kp * (obj->err_k - obj->err_k1) +
                     obj->ki * obj->err_k +
                     obj->kd * (obj->err_k - 2 * obj->err_k1 + obj->err_k2);

    obj->u_k = obj->u_k1 + obj->delta_u_k;
    return obj->u_k;
}
