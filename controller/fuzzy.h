/*
 * @Author: luoqi 
 * @Date: 2021-09-16 11:44:17 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 16:22:25
 */

#ifndef _FUZZY_H
#define _FUZZY_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "../frame/qlist.h"

typedef enum _fuzz_mf_var
{
    FUZZ_E,
    FUZZ_EC,
    FUZZ_U
} FuzzMfVar;

typedef enum _membership_function_type
{
    FUZZ_TRIMF,     // Triangular membership function
    FUZZ_TRAPMF,    // Tampezoid membership function
    FUZZ_PIMF,      // Pi-shaped membership function
    FUZZ_ZMF,       // Z-shaped membership function
    FUZZ_SMF,       // S-shaped function

    FUZZ_GBELLMF,   // Generalized bell-shaped membership function
    FUZZ_GAUSSMF,   // Gaussian membership function
    FUZZ_GAUSS2MF,  // Gaussian combination membership function
    FUZZ_SIGMF,     // Sigmoidal membership function
    FUZZ_DSIGMF,    // Difference between two sigmoidal membership function
    FUZZ_PSIGMF,    // Product of two sigmoidal membership functions
} FuzzMemFuncType;

typedef enum _fuzz_domain
{
    FUZZ_NB = -3,
    FUZZ_NM = -2,
    FUZZ_NS = -1,
    FUZZ_ZO = 0,
    FUZZ_PS = 1,
    FUZZ_PM = 2,
    FUZZ_PB = 3,
} FuzzDesc;

typedef enum _fuzz_method
{
    FUZZ_METHOD_SELF_DEF,
} FuzzMethod;

typedef enum _defuzz_method
{
    DFUZZ_METHOD_MOM,        // maximum membership average method
    DFUZZ_METHOD_CENTROID,   // area center of gravity method
    DFUZZ_METHOD_BISECTOR,   // area dividing method
    DFUZZ_METHOD_SOM,        // smallest method of maximum membership
    DFUZZ_METHOD_LOM         // maximum membership degree to dafa
} DefuzzMethod;
typedef struct _fuzz_membership_function_object
{
    const char *name;
    FuzzMemFuncType type;
    float param_a; // sigma  | sigma1 | ak | ak1
    float param_b; // c      | c1     | ck | ck1
    float param_c; // sigma2 | ak2
    float param_d; // c2     | ck2
    ListObj _mf_inner_list;
} FuzzMemFuncObj;

typedef struct _fuzz_controller_object
{
    float e_range[2];
    float ec_range[2];
    float u_range[2];
    FuzzMethod fuzz_method;
    DefuzzMethod defuzz_method;

    int dis_e_size;
    int dis_ec_size;
    int dis_u_size;
    const FuzzDesc **rules;

    int mf_e_num;
    int mf_ec_num;
    int mf_u_num;
    ListObj _mf_list_e;
    ListObj _mf_list_ec;
    ListObj _mf_list_u;
} FuzzObj;

void fuzz_init(FuzzObj* fuzz_obj,
               float e_min, float e_max,
               float ec_min, float ec_max,
               float u_min, float u_max,
               int dis_e_size,
               int dis_ec_size,
               int dis_u_size,
               const FuzzDesc **rules,
               FuzzMethod fuzz_method,
               DefuzzMethod defuzz_method);

int fuzz_mf_init(FuzzMemFuncObj* mf, const char *name, FuzzMemFuncType mf_type, float *param, int param_num);

int fuzz_mf_add(FuzzObj* fuzz_obj, FuzzMfVar var, FuzzMemFuncObj* mf);

int fuzz_mf_num(FuzzObj* fuzz_obj, FuzzMfVar var);

float fuzz_control_2d(FuzzObj* fuzz_obj, float e, float ec);

float fuzz_control_1d(FuzzObj* fuzz_obj, float e);

#ifdef __cplusplus
 }
#endif

#endif
