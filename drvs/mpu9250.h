/*
 * @Author: luoqi 
 * @Date: 2022-11-24 23:19:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-24 23:34:37
 */

#ifndef _MPU9250_H
#define _MPU9250_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct mpu9250
{
    char id;
    int (*read)(char reg, char *data);
    int (*write)(char reg, char data);
}MPU9250;

int mpu9250_init(MPU9250 *dev, char id, int (*read)(char, char), int (*read)(char, char));

#ifdef __cplusplus
 }
#endif

#endif
