/*
 * @Author: luoqi 
 * @Date: 2021-09-16 11:44:17 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-09-16 19:18:02
 */

#ifndef _FUZZY_H
#define _FUZZY_H

#include "../kernel/service/list.h"

typedef enum _defuzz_method
{
    FUZZ_MOM,        // maximum membership average method
    FUZZ_CENTROID,   // area center of gravity method
    FUZZ_BISECTOR,   // area dividing method
    FUZZ_SOM,        // smallest method of maximum membership
    FUZZ_LOM         // maximum membership degree to dafa
} DefuzzMethod;

typedef enum _membership_function_type
{
    FUZZ_TRIAMF,    // Triangular membership function
    FUZZ_TRAPMF,    // Tampezoid membership function
    FUZZ_GBELLMF,   // Generalized bell-shaped membership function
    FUZZ_GAUSSMF,   // Gaussian membership function
    FUZZ_GAUSS2MF,  // Gaussian combination membership function
    FUZZ_SIGMF,     // Sigmoidal membership function
    FUZZ_DSIGMF,    // Difference between two sigmoidal membership function
    FUZZ_PSIGMF,    // Product of two sigmoidal membership functions
    FUZZ_PIMF,      // Pi-shaped membership function
    FUZZ_SMF,       // D-shaped function
    FUZZ_ZMF        // Pi-shaped membership function
} FuzzMemFuncType;

typedef struct _fuzz_membership_function_object
{
    const char* name;
    DefuzzMethod method;
    float (*mf)(float);
    ListObj _mf_inner_list;
} FuzzMemFuncObj;


typedef struct _fuzz_controller_object
{
    float fuzz_input_range[2];
    float fuzz_output_range[2];
    DefuzzMethod defuzz_method;

    ListObj _fuzz_mf_list;
} FuzzObj;

void fuzz_init(FuzzObj* obj,
               float input_min, float input_max,
               float output_min, float output_max,
               DefuzzMethod defuzz_method);

FuzzMemFuncObj fuzz_mf_creat(FuzzMemFuncType mf_type);

void fuzz_mf_init(FuzzMemFuncObj* obj, float (*mf)(float), const char* name);

void fuzz_mf_add(FuzzObj* obj, FuzzMemFuncObj* mf);

float fuzz_control(FuzzObj* obj, float err);


#endif
