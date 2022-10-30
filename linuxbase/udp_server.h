/*
 * @Author: luoqi 
 * @Date: 2022-09-05 10:51:56 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-09-05 11:24:58
 */

#ifndef _UDP_SERVER_H
#define _UDP_SERVER_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "sys/socket.h"

int udp_server_creat(int port);

int udp_server_delete(int fd);

int udp_server_send(int fd, void *msg, int msg_size, struct sockaddr *dst, socklen_t dst_len);

int udp_server_recv(int fd, void *buf, int buf_size, struct sockaddr *src, socklen_t *src_len, int wait_usec);

#ifdef __cplusplus
 }
#endif

#endif
