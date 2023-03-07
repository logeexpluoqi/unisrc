/*
 * @Author: luoqi 
 * @Date: 2023-03-07 20:38:56 
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-03-07 22:02:28
 */

#include <math.h>
#include "qrot.h"

#define qrot_sin(x)         sinf(x)
#define qrot_cos(x)         cosf(x)
#define qrot_atan2(y, x)    atan2f(y, x)
#define qrot_asin(x)        asinf(x)

QRotCoord3D qrot_euler(QRotCoord3D c, QRotEuler e)
{
    QRotCoord3D _c;
    _c.x = c.x * (qrot_cos(e.alpha) * qrot_cos(e.gamma) - qrot_sin(e.alpha) * qrot_cos(e.beta) * qrot_sin(e.gamma)) 
           + c.y * (-qrot_sin(e.alpha) * qrot_cos(e.gamma) - qrot_cos(e.alpha) * qrot_cos(e.beta) * qrot_sin(e.gamma))
           + c.z * (qrot_sin(e.beta) * qrot_sin(e.gamma));
    _c.y = c.x * (qrot_cos(e.alpha) * qrot_sin(e.gamma) + qrot_sin(e.alpha) * qrot_cos(e.beta) * qrot_cos(e.gamma)) 
           + c.y * (-qrot_sin(e.alpha) * qrot_sin(e.gamma) + qrot_cos(e.alpha) * qrot_cos(e.beta) * qrot_cos(e.gamma))
           + c.z * (-qrot_sin(e.beta) * qrot_cos(e.gamma));
    _c.z = c.x * (qrot_sin(e.alpha) * qrot_sin(e.beta))
           + c.y * (qrot_cos(e.alpha) * qrot_sin(e.beta))
           + c.z * qrot_cos(e.beta);

    return _c;
}

QRotCoord3D qrot_quat(QRotCoord3D c, QRotQuat q)
{
    QRotCoord3D _c;

    return _c;
}

QRotQuat qrot_e2q(QRotEuler e)
{
    QRotQuat q;
    float alpha = e.alpha / 2;
    float beta = e.beta/ 2;
    float gamma = e.gamma / 2;
    q.w = qrot_cos(alpha) * qrot_cos(beta) * qrot_cos(gamma) + qrot_sin(alpha) * qrot_sin(beta) * qrot_sin(gamma);
    q.i = qrot_sin(alpha) * qrot_cos(beta) * qrot_cos(gamma) - qrot_cos(alpha) * qrot_sin(beta) * qrot_sin(gamma);
    q.j = qrot_cos(alpha) * qrot_sin(beta) * qrot_cos(gamma) + qrot_sin(alpha) * qrot_cos(beta) * qrot_sin(gamma);
    q.k = qrot_cos(alpha) * qrot_cos(beta) * qrot_sin(gamma) - qrot_sin(alpha) * qrot_sin(beta) * qrot_cos(gamma);

    return q;
}

QRotEuler qrot_q2e(QRotQuat q)
{
    QRotEuler e;
    e.alpha = qrot_atan2(2 * (q.w * q.i + q.j * q.k), 1 - 2 * (q.i * q.i + q.j * q.j));
    e.beta = qrot_asin(2 * (q.w * q.j - q.i * q.k));
    e.gamma = qrot_atan2(2 * (q.w * q.k + q.i * q.j), 2 * (q.j * q.j + q.k * q.k));

    return e;
}
