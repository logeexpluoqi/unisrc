/*
 * @Author: luoqi
 * @Date: 2022-11-24 23:19:26
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-24 23:36:50
 */

#ifndef _DS18B20_H
#define _DS18B20_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ds18b20
{
    char id;
    int (*read)(char reg, char* data);
    int (*write)(char reg, char data);
}DS18B20;

int ds18b20_init(DS18B20* dev, char id, int (*read)(char, char), int (*read)(char, char));

#ifdef __cplusplus
}
#endif

#endif
