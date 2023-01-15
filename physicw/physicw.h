/*
 * @Author: luoqi 
 * @Date: 2022-11-10 23:15:52 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-10 23:20:43
 */

#ifndef _PHYSICW_H
#define _PHYSICW_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct physicw
{
    float gravity;
    float wind[2];
} PhysicWObj;

int phyw_init(PhysicWObj *world, float gravity);

int phyw_wind_set(PhysicWObj *world, float wind[2]);

#ifdef __cplusplus
 }
#endif

#endif
