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
#include "database.h"
#include <ctime>

#define BUF_SIZE 1024

using namespace std;

int main()
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    pthread_t tid;
    char msg[BUF_SIZE];
    char query[100];
    time_t timer;
    struct tm* t;
    Database DB;

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

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock < 0)
    {
        perror("accept");
        return -1;
    }
    printf("Connected client IP : %s\n", inet_ntoa(clnt_addr.sin_addr));

    while (recv(clnt_sock, msg, sizeof(msg), 0) != 0)
    {
        timer = time(NULL);
        t = localtime(&timer);
        string date_data = to_string(t->tm_year + 1900) + "-" + to_string(t->tm_mon + 1) + "-" + to_string(t->tm_mday);
        string time_data = to_string(t->tm_hour) + ":" + to_string(t->tm_min);
        sprintf(query, "INSERT INTO current (plate, date_in, time_in) VALUES ('%s', '%s', '%s')", msg, date_data.c_str(), time_data.c_str());
        if (mysql_query(&DB.conn, query) != 0)
                fprintf(stderr, "database connect error : %s\n", mysql_error(&DB.conn));
        else
            cout << "sucess" << endl;
        memset(msg, 0, sizeof(msg));
    }

    close(serv_sock);
    return 0;
}
