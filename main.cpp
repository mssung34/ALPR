#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define MAX_CLNT 5

void *handle_clnt(void *arg);
void send_msg(char *msg, int len);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];

pthread_mutex_t mtx;

int main()
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    pthread_t tid;

    pthread_mutex_init(&mtx, NULL);
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock < 0)
    {
        perror("socket");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(9022);

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        return -1;
    }

    if (listen(serv_sock, 5) < 0)
    {
        perror("listen");
        return -1;
    }

    while (1)
    {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock < 0)
        {
            perror("accept");
            return -1;
        }

        pthread_mutex_lock(&mtx);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mtx);

        pthread_create(&tid, NULL, handle_clnt, (void *)&clnt_sock);
        pthread_detach(tid);
        printf("Connected client IP : %s\n", inet_ntoa(clnt_addr.sin_addr));
    }

    close(serv_sock);
    return 0;
}

void *handle_clnt(void *arg)
{
    int clnt_sock = *((int *)arg);
    int str_len = 0;
    char msg[BUF_SIZE];
    char who[BUF_SIZE];

    printf("con sock: %d\n", clnt_sock);

    //recv(clnt_sock, who, sizeof(msg), 0);

    //std::cout << who << std::endl;
    //std::cout << sizeof(who) << std::endl;

    while (recv(clnt_sock, msg, sizeof(msg), 0) != 0)
    {
        std::cout << msg << std::endl;
        for (int i = 0; i < clnt_cnt; i++)
        {
            send(clnt_socks[i], msg, sizeof(msg), 0);
        }
        memset(msg, 0, sizeof(msg));
    }
    pthread_mutex_lock(&mtx);
    for (int i = 0; i < clnt_cnt; i++)
    {
        if (clnt_sock == clnt_socks[i])
        {
            while (++i < clnt_cnt)
            {
                clnt_socks[i - 1] = clnt_socks[i];
            }
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mtx);

    close(clnt_sock);
    return NULL;
}
