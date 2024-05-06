/*
 * @Author: luoqi 
 * @Date: 2021-09-16 11:45:00 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-09-23 17:50:03
 */

#include "fuzzy.h"

static float _tri_memfunc(float x, float a, float b, float c);
static float _trap_memfunc(float x, float a, float b, float c, float d);
static float _pi_memfunc(float x, float a, float b, float c, float d);
static float _s_memfunc(float x, float a, float b);
static float _z_memfunc(float x, float a, float b);
static float _gbell_memfunc(float x, float a, float b, float c);
static float _gauss_memfunc(float x, float sigma, float c);
static float _gauss2_memfunc(float x, float sigma1, float c1, float sigma2, float c2);
static float _sig_memfunc(float x, float ak, float ck);
static float _dsig_memfunc(float x, float ak1, float ck1, float ak2, float ck2);
static float _psig_memfunc(float x, float ak1, float ck1, float ak2, float ck2);

static float _memfunc_output(FuzzMemFuncObj (*), float);

static FuzzDesc _fuzzification(FuzzMethod method, float x);


int fuzz_mf_init(FuzzMemFuncObj* mf, const char *name, FuzzMemFuncType mf_type, float *param, int param_num)
{
    mf->type = mf_type;
    mf->name = name;
    switch(mf_type)
    {
    case FUZZ_TRIMF:
        if(param_num != 3)
            return -1;

        mf->param_a = param[0];
        mf->param_b = param[1];
        mf->param_c = param[2];
        mf->param_d = 0;
        break;
    case FUZZ_TRAPMF:
        if(param_num != 4)
            return -1;

        mf->param_a = param[0];
        mf->param_b = param[1];
        mf->param_c = param[2];
        mf->param_d = param[3];
        break;
    case FUZZ_PIMF:
        if(param_num != 4)
            return -1;
        mf->param_a = param[0];
        mf->param_b = param[1];
        mf->param_c = param[2];
        mf->param_d = param[3];
        break;
    case FUZZ_SMF:
        if(param_num != 3)
            return -1;
        
        mf->param_a = param[0];
        mf->param_b = param[1];
        mf->param_c = param[2];
        mf->param_d = 0;
        break;
    case FUZZ_ZMF:
        if(param_num != 3)
            return -1;

        mf->param_a = param[0];
        mf->param_b = param[1];
        mf->param_c = param[2];
        mf->param_d = 0;
        break;
    case FUZZ_GBELLMF:
        if(param_num != 3)
            return -1;
        mf->param_a = param[0];
        mf->param_b = param[1];
        mf->param_c = param[2];
        mf->param_d = 0;
        break;
    case FUZZ_GAUSSMF:
        if(param_num != 2)
            return -1;

        mf->param_a = param[0];
        mf->param_b = param[1];
        mf->param_c = 0;
        mf->param_d = 0;
        break;
    case FUZZ_GAUSS2MF:
        if(param_num != 4)
            return -1;

        mf->param_a = param[0];
        mf->param_b = param[1];
        mf->param_c = param[2];
        mf->param_d = param[3];
        break;
    case FUZZ_SIGMF:
        if(param_num != 2)
            return -1;

        mf->param_a = param[0];
        mf->param_b = param[1];
        mf->param_c = 0;
        mf->param_d = 0;
        break;
    case FUZZ_DSIGMF:
        if(param_num != 4)
            return -1;

        mf->param_a = param[0];
        mf->param_b = param[1];
        mf->param_c = param[2];
        mf->param_d = param[3];
        break;
    case FUZZ_PSIGMF:
        if(param_num != 4)
            return -1;

        mf->param_a = param[0];
        mf->param_b = param[1];
        mf->param_c = param[2];
        mf->param_d = param[3];
        break;
    default:
        return -1;
        break;
    }
    
    return 0;
}

void fuzz_init(FuzzObj* fuzz_obj,
               float e_min, float e_max,
               float ec_min, float ec_max,
               float u_min, float u_max,
               int dis_e_size,
               int dis_ec_size,
               int dis_u_size,
               const FuzzDesc **rules,
               FuzzMethod fuzz_method,
               DefuzzMethod defuzz_method)
{
    fuzz_obj->mf_e_num = 0;
    fuzz_obj->mf_ec_num = 0;
    fuzz_obj->mf_u_num = 0;
    fuzz_obj->dis_e_size = dis_e_size;
    fuzz_obj->dis_ec_size = dis_ec_size;
    fuzz_obj->dis_u_size = dis_u_size;
    fuzz_obj->rules = rules;
    qlist_init(&fuzz_obj->_mf_list_e);
    qlist_init(&fuzz_obj->_mf_list_ec);
    qlist_init(&fuzz_obj->_mf_list_u);
    fuzz_obj->e_range[0] = e_min;
    fuzz_obj->e_range[1] = e_max;
    fuzz_obj->ec_range[0] = ec_min;
    fuzz_obj->ec_range[1] = ec_max;
    fuzz_obj->u_range[0] = u_max;
    fuzz_obj->u_range[1] = u_max;
    fuzz_obj->fuzz_method = fuzz_method;
    fuzz_obj->defuzz_method = defuzz_method;
}

float fuzz_control_2d(FuzzObj* fuzz_obj, float e, float ec)
{
    float uk;

    return uk;
}

float fuzz_control_1d(FuzzObj* fuzz_obj, float e)
{
    float uk;
    


    return uk;
}

int fuzz_mf_add(FuzzObj* fuzz_obj, FuzzMfVar var, FuzzMemFuncObj* mf)
{
    switch(var)
    {
    case FUZZ_E:
        qlist_insert_before(&fuzz_obj->_mf_list_e, &mf->_mf_inner_list);
        fuzz_obj->mf_e_num ++;
        break;
    case FUZZ_EC:
        qlist_insert_before(&fuzz_obj->_mf_list_ec, &mf->_mf_inner_list);
        fuzz_obj->mf_ec_num ++;
        break;
    case FUZZ_U:
        qlist_insert_before(&fuzz_obj->_mf_list_u, &mf->_mf_inner_list);
        fuzz_obj->mf_u_num ++;
        break;
    default:
        return -1;
        break;
    }
    return 0;
}

int fuzz_mf_num(FuzzObj* fuzz_obj, FuzzMfVar var)
{
    if(var == FUZZ_E)
        return qlist_length(&fuzz_obj->_mf_list_e);
    else if(FUZZ_EC)
        return qlist_length(&fuzz_obj->_mf_list_ec);
    else if(FUZZ_U)
        return qlist_length(&fuzz_obj->_mf_list_u);
    else
        return -1;
}

FuzzDesc _fuzzification(FuzzMethod method, float x)
{
    
    
}

float _memfunc_output(FuzzMemFuncObj *mf, float x)
{
    switch(mf->type)
    {
    case FUZZ_TRIMF:
        return _tri_memfunc(x, mf->param_a, mf->param_b, mf->param_c);
    case FUZZ_TRAPMF:
        return _trap_memfunc(x, mf->param_a, mf->param_b, mf->param_c, mf->param_d);
    case FUZZ_PIMF:
        return _pi_memfunc(x, mf->param_a, mf->param_b, mf->param_c, mf->param_d);
    case FUZZ_ZMF:
        return _z_memfunc(x, mf->param_a, mf->param_b);
    case FUZZ_SMF:
        return _s_memfunc(x, mf->param_a, mf->param_b);
    case FUZZ_GBELLMF:
        return _gbell_memfunc(x, mf->param_a, mf->param_b, mf->param_c);
    case FUZZ_GAUSSMF:
        return _gauss_memfunc(x, mf->param_a, mf->param_b);
    case FUZZ_GAUSS2MF:
        return _gauss2_memfunc(x, mf->param_a, mf->param_b, mf->param_c, mf->param_d);
    case FUZZ_SIGMF:
        return _sig_memfunc(x, mf->param_a, mf->param_b);
    case FUZZ_DSIGMF:
        return _dsig_memfunc(x, mf->param_a, mf->param_b, mf->param_c, mf->param_d);
    case FUZZ_PSIGMF:
        return _psig_memfunc(x, mf->param_a, mf->param_b, mf->param_c, mf->param_d);
    default:
        break;
    }
    return 0;
}

float _tri_memfunc(float x, float a, float b, float c)
{
    if(x <= a)
        return 0;
    else if(x > a && x <= b)
        return (x - a) / (b - a);
    else if(x > b && x <= c)
        return (c - x) / (c - b);
    else
        return 0;
}

float _trap_memfunc(float x, float a, float b, float c, float d)
{
    if(x <= a)
        return 0;
    else if(x > a && x <= b)
        return (x - a) / (b - a);
    else if(x > b && x <= c)
        return 1;
    else if(x > c && x <= d)
        return (d - x) / (d - c);
    else
        return 0;   
}

float _pi_memfunc(float x, float a, float b, float c, float d)
{
    if(x <= a)
        return 0;
    else if(x > a && x <= (a + b) / 2)
        return 2 * ((x - a) / (b - a)) * ((x - a) / (b - a));
    else if(x > (a + b) / 2 && x <= b)
        return 1 - 2 * ((x - b) / (b - a)) * ((x - b) / (b - a));
    else if(x > b && x <= c)
        return 1;
    else if(x > c && x <= (c + d) / 2)
        return 1 - 2 * ((x - c) / (d - c)) * ((x - c) / (d - c));
    else if(x > (c + d) / 2)
        return 2 * ((x - d) / (d - c)) * ((x - d) / (d - c));
    else 
        return 0;
}

float _s_memfunc(float x, float a, float b)
{
    if(x <= a)
        return 0;
    else if(x > a && x <= (a + b) / 2)
        return 2 * ((x - a) / (b - a)) * ((x - a) / (b - a));
    else if( x > (a + b) / 2)
        return 1 - 2 * ((x - b) / (b - a)) * ((x - b) / (b - a));
    else 
        return 0;
}

float _z_memfunc(float x, float a, float b)
{
    if(x <= a)
        return 0;
    else if(x > a && x <= (a + b) / 2)
        return 1 - 2 * ((x - a) / (b - a)) * ((x - a) / (b - a));
    else if( x > (a + b) / 2)
        return 2 * ((x - b) / (b - a)) * ((x - b) / (b - a));
    else 
        return 0;
}

float _gbell_memfunc(float x, float a, float b, float c)
{
    return 0;
}

float _gauss_memfunc(float x, float sigma, float c)
{
    return 0;
}

float _gauss2_memfunc(float x, float sigma1, float c1, float sigma2, float c2)
{
    return 0;
}

float _sig_memfunc(float x, float ak, float ck)
{
    return 0;
}

float _dsig_memfunc(float x, float ak1, float ck1, float ak2, float ck2)
{
    return 0;
}

float _psig_memfunc(float x, float ak1, float ck1, float ak2, float ck2)
{
    return 0;
}
