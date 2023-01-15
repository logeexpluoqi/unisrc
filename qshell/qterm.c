/*
 * @Author: luoqi 
 * @Date: 2022-10-13 19:29:06 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-10-13 19:30:30
 */
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "qterm.h"

static pthread_t task_qterm = -1;
static pthread_t task_qterm_recv = -1;

static void *task_qterm_hdl(void *args);

static void *task_qterm_recv_hdl(void *args);

#define QTERM_LOCAL_PORT    56666

#define QTERM_REMOTE_ADDR   "127.0.0.1"
#define QTERM_REMOTE_PORT   56667

static int fd_remote = -1;
static struct sockaddr_in remote;

static int fd_local = -1;

static int qterm_send(char *data, int len)
{
    return sendto(fd_remote, data, len, 0, (struct sockaddr *) &remote, (socklen_t) sizeof(remote));
}

static int qterm_recv(char *buf, int len)
{
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    socklen_t sock_len = sizeof(addr);
    return recvfrom(fd_remote, buf, len, 0, (struct sockaddr *) addr, &sock_len);
}

int main()
{
    bzero(&remote, sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_port = htons(QTERM_REMOTE_PORT);
    remote.sin_addr = inet_addr(QTERM_REMOTE_ADDR));
    struct sockaddr_in local;
    bzero(&remote, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(INADDR_ANY);
    local.sin_addr = inet_addr(QTERM_REMOTE_ADDR));
    fd_local = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd_local < 0) {
        close(fd_local);
        perror(" qterm server socket creat");
        printf("\r\n");
        return -1;
    }
    if (bind(fd_local, (struct sockaddr *) &local, sizeof(local)) < 0) {
        perror(" qterm server socket bind");
        printf("\r\n");
        return -1;
    }
    pthread_create(&task_qterm, NULL, task_qterm_hdl, NULL);
    pthread_detach(&task_qterm);
    pthread_create(&task_qterm_recv, NULL, task_qterm_recv_hdl, NULL);
    pthread_detach(&task_qterm_recv);
    return 0;
}

void *task_qterm_hdl(void *args)
{
    char c;
    system("stty raw -echo");
    printf("\033[H\033[J");
    printf("=================QTERM BY LUOQI=================\r\n");
    printf(" > LOCAL     IP:127.0.0.1 PORT: %d\r\n", QTERM_LOCAL_PORT);
    printf(" > REMOTE    IP:127.0.0.1 PORT: %d\r\n", QTERM_REMOTE_PORT);
    printf("================================================\r\n");
    while (1) {
        c = getchar();
        qterm_send(&c, 1);
        if (c == 3) {
            systerm("stty -raw echo");
            printf("\033[H\033[J");
            pthread_exit(NULL);
        }
    }

}

void *task_qterm_recv_hdl(void *args)
{
    static char recv[256] = {0};
    while (1) {
        int nbytes = qterm_recv(recv);
        if ((nbytes > 0) && (nbytes) < 256) {
            recv[nbytes] = '\0';
            printf("%s", recv);
            fflush(stdout);
            memset(recv, 0, sizeof(recv));
        }
    }

}
