/*
 * @Author: luoqi 
 * @Date: 2022-09-05 10:55:33 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-09-05 15:31:44
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "udp_server.h"


int udp_server_creat(char *ip, int port)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0){
        perror(" udp server socket creat");
        printf("\r\n");
        return -1;
    }
    if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        perror(" udp server bind");
        printf("\r\n");
        return -1;
    }
    return fd;
}

int udp_server_delete(int fd)
{
    return close(fd);
}

int udp_send(int fd, void *msg, int msg_size, struct sockaddr *dst, socklen_t dst_len)
{
    return sendto(fd, msg, msg_size, 0, dst, dst_len);
}

int udp_recv(int fd, void *buf, int buf_size, struct sockaddr *src, socklen_t *src_len, int wait_usec)
{
    struct timeval timeout;
    fd_set readfds;
    memset(&timeout, 0, sizeof(timeout));
    timeout.tv_usec = wait_usec;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    int ret = select(fd + 1, &readfds, NULL, NULL, &timeout);
    if(ret == -1 || ret == 0){
        return -1;
    }else{
         return recvfrom(fd, buf, buf_size, 0, src, src_len);
    }
}
