/*
 * @Author: luoqi 
 * @Date: 2023-03-07 20:38:56 
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-03-07 21:41:11
 */

#include <math.h>
#include "qrot.h"

QRotCoord3D qrot_euler(QRotCoord3D c, QRotEuler e)
{
    QRotCoord3D _c;
    _c.x = c.x * (cosf(e.alpha) * cosf(e.gamma) - sinf(e.alpha) * cosf(e.beta) * sinf(e.gamma)) 
           + c.y * (-sinf(e.alpha) * cosf(e.gamma) - cosf(e.alpha) * cosf(e.beta) * sinf(e.gamma))
           + c.z * (sinf(e.beta) * sinf(e.gamma));
    _c.y = c.x * (cosf(e.alpha) * sinf(e.gamma) + sinf(e.alpha) * cosf(e.beta) * cosf(e.gamma)) 
           + c.y * (-sinf(e.alpha) * sinf(e.gamma) + cosf(e.alpha) * cosf(e.beta) * cosf(e.gamma))
           + c.z * (-sinf(e.beta) * cosf(e.gamma));
    _c.z = c.x * (sinf(e.alpha) * sinf(e.beta))
           + c.y * (cosf(e.alpha) * sinf(e.beta))
           + c.z * cosf(e.beta);

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
    q.w = cosf(alpha) * cosf(beta) * cosf(gamma) + sinf(alpha) * sinf(beta) * sinf(gamma);
    q.i = sinf(alpha) * cosf(beta) * cosf(gamma) - cosf(alpha) * sinf(beta) * sinf(gamma);
    q.j = cosf(alpha) * sinf(beta) * cosf(gamma) + sinf(alpha) * cosf(beta) * sinf(gamma);
    q.k = cosf(alpha) * cosf(beta) * sinf(gamma) - sinf(alpha) * sinf(beta) * cosf(gamma);

    return q;
}

QRotEuler qrot_q2e(QRotQuat q)
{
    QRotEuler e;
    e.alpha = atan2f(2 * (q.w * q.i + q.j * q.k), 1 - 2 * (q.i * q.i + q.j * q.j));
    e.beta = asinf(2 * (q.w * q.j - q.i * q.k);
    e.gamma = atan2f(2 * (q.w * q.k + q.i * q.j), 2 * (q.j * q.j + q.k * q.k));

    return e;
}
