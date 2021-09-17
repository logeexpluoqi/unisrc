/*
 * @Author: luoqi 
 * @Date: 2021-09-16 11:45:00 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-09-17 18:18:31
 */

#include "fuzzy.h"

typedef struct _trimf
{
    float a, b, c;
    float (*trimf)(float);
} Trimf;

typedef struct _trapmf
{
    float a, b, c, d;
    float (*trapmf)(float);
} Trapmf;

typedef struct _pimf
{
    float a, b, c, d;
    float (*pimf)(float);
} Pimf;

typedef struct _smf
{
    float a, b;
    float (*smf)(float);
} Smf;

typedef struct _zmf
{
    float a, b;
    float (*zmf)(float);
} Zmf;

typedef struct _gbellmf
{
    float a, b, c;   
    float (*gbellmf)(float);
} Gbellmf;

typedef struct _gaussmf
{
    float sigma, c;
    float (*gaussmf)(float);
} Gaussmf;

typedef struct _gauss2mf
{
    float sigma1, c1, sigma2, c2;
    float (*gauss2mf)(float);
} Gauss2mf;

typedef struct _sigmf
{
    float ak, ck;
    float (*sigmf)(float);
} Sigmf;

typedef struct _dsigmf
{
    float ak1, ck1, ak2, ck2;
    float (*dsigmf)(float);
} DSigmf;

typedef struct _psigmf
{
    float ak1, ck1, ak2, ck2;
    float (*psigmf)(float);
} PSigmf;

static float _tri_memfunc(float x, float a, float b, float c);
static float _trap_memfunc(float x, float a, float b, float c, float d);
static float _pimf_memfunc(float x, float a, float b, float c, float d);
static float _smf_memfunc(float x, float a, float b);
static float _zmf_memfunc(float x, float a, float b);
static float _gbell_memfunc(float x, float a, float b, float c);
static float _gauss_memfunc(float x, float sigma, float c);
static float _gauss2_memfunc(float x, float sigma1, float c1, float sigma2, float c2);
static float _sig_memfunc(float x, float ak, float ck);
static float _dsig_memfunc(float x, float ak1, float ck1, float ak2, float ck2);
static float _psig_memfunc(float x, float ak1, float ck1, float ak2, float ck2);


int fuzz_mf_init(FuzzMemFuncObj* mf, FuzzMemFuncType mf_type, float *param, int param_num)
{
    mf->type = mf_type;

    switch(mf_type)
    {
    case FUZZ_TRIMF:
        break;
    case FUZZ_TRAPMF:
        break;
    case FUZZ_PIMF:
        break;
    case FUZZ_SMF:
        break;
    case FUZZ_ZMF:
        break;
    case FUZZ_GBELLMF:
        break;
    case FUZZ_GAUSSMF:
        break;
    case FUZZ_GAUSS2MF:
        break;
    case FUZZ_SIGMF:
        break;
    case FUZZ_DSIGMF:
        break;
    case FUZZ_PSIGMF:
        break;
    default:
        break;
    }
    
    return 0;
}

void fuzz_init(FuzzObj* fuzz_obj,
               float input_min, float input_max,
               float output_min, float output_max,
               DefuzzMethod defuzz_method)
{

}

float fuzz_control(FuzzObj* fuzz_obj, float err)
{
    float y_k;

    return y_k;
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

float _pimf_memfunc(float x, float a, float b, float c, float d)
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

float _smf_memfunc(float x, float a, float b)
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

float _zmf_memfunc(float x, float a, float b)
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
