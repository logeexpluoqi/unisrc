/*
 * @Author: luoqi 
 * @Date: 2023-03-07 20:36:07 
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-03-07 21:41:01
 */

#ifndef _QROT_H
#define _QROT_H
#ifdef __cplusplus
 extern "C" {
#endif

typedef struct qrot_coordinate_3d
{
    float x;
    float y;
    float z;
} QRotCoord3D;

typedef struct qrot_euler_angle
{
    float alpha;
    float beta;
    float gamma;
} QRotEuler;

typedef struct qrot_quatrnion
{
    float i;
    float j;
    float k;
    float w;
} QRotQuat;

QRotCoord3D qrot_euler(QRotCoord3D c, QRotEuler e);

QRotCoord3D qrot_quat(QRotCoord3D c, QRotQuat q);

QRotQuat qrot_e2q(QRotEuler e);

QRotEuler qrot_q2e(QRotQuat q);

#ifdef __cplusplus
 }
#endif
#endif
