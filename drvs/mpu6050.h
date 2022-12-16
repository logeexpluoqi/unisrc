/*
 * @Author: luoqi 
 * @Date: 2022-11-24 23:20:16 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-28 00:05:20
 */

#ifndef _MPU6050_H
#define _MPU9250_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct mpu6050
{
    char id;
    int (*read)(char addr, char reg, char *data);
    int (*write)(char addr, char reg, char data);
}MPU6050;

int mpu6050_init(MPU6050 *dev, char id, int (*read)(char, char), int (*read)(char, char));

#ifdef __cplusplus
 }
#endif

#endif
