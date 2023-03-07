/*
 * @Author: luoqi 
 * @Date: 2023-03-07 20:38:56 
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-03-07 21:14:38
 */

#include <math.h>
#include "qrot.h"

QRotCoord3D qrot_euler(QRotCoord3D c, QRotEulerAngle e)
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
