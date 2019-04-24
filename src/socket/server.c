#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#include "touch_send_event.h"
#include "log.h"
#include "server.h"
#include "protocol_atouch.h"
#include "protocol_adb.h"

#define BUF_SIZE 2048
#define PORT 1989

char server_loop = 1;
int sock_fd;
pthread_t server_thread, client_thread;
int socket_atouch = -1, socket_adb = -1;

void socket_atouch_send(char *buf, int len)
{
    if (socket_atouch != -1)
    {
        if (send(socket_atouch, buf, len, 0) < 0)
        {
            socket_atouch = -1;
        }
    }
}

void socket_adb_send(char *buf, int len)
{
    if (socket_adb != -1)
    {
        if (send(socket_adb, buf, len, 0) < 0)
        {
            socket_adb = -1;
        }
    }
}

void *client_fun_thread(void *arg)
{
    static char is_init = 1;
    static uint8_t test_count2 = 0;

    int ret = -1;
    int conn_fd = *(int *)arg;
    char buff[BUF_SIZE];
    char client_loop = 1;
    char witch_connect = 0;

    printf("Client Connect %d\r\n", conn_fd);

    while (client_loop)
    {
        // 接受数据
        ret = recv(conn_fd, buff, BUF_SIZE, 0);
        // log_byte(buff,ret);
        //     if(is_init == 1)
        //     {
        //         is_init = 0;
        //         test_count2 = buff[ret - 1];
        //     }else{
        //         test_count2++;
        //         if(test_count2 != buff[ret - 1])
        //         {
        //             test_count2 = buff[ret - 1];
        //             LOG("=============>ERROR");
        //         }
        //     }

        if (ret <= 0)
        {
            // 客户端关闭
            //printf("client close\n");
            close(conn_fd);
            client_loop = 0;
        }
        else
        {
            if (witch_connect == 0)
            {
                if (atouch_receive(buff, ret))
                {
                    socket_atouch = conn_fd;
                    witch_connect = 1;
                }

                if (adb_receive(buff, ret))
                {
                    socket_adb = conn_fd;
                    witch_connect = 2;
                }
            }
            else if (witch_connect == 1)
            {
                atouch_receive(buff, ret);
                adb_receive(buff, ret);
            }
            else if (witch_connect == 2)
            {
                adb_receive(buff, ret);
            }
        }
    }

    printf("Client Disconnect\r\n");
}

void *server_fun_thread(void *arg)
{
    int ret = -1;
    int conn_fd;

    while (server_loop)
    {

        // 接受请求
        if ((conn_fd = accept(sock_fd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            printf("accept socket Fail\r\n");
            continue;
        }

        pthread_create(&client_thread, NULL, client_fun_thread, &conn_fd);
        //close(conn_fd);
    }
}

int server_init(void)
{

    struct sockaddr_in server_addr;

    int ret = -1, on = 1;

    // 创建socket描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Socket Creat Fail\r\n");
        return ret;
    }

    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    // 填充sockaddr_in结构
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // 绑定sock_fd描述符
    if (bind(sock_fd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
    {
        printf("Bind Fail\r\n");
        return ret;
    }

    // 监听sock_fd描述符
    if (listen(sock_fd, 5) == -1)
    {
        printf("Listen Fail\r\n");
        return ret;
    }

    pthread_create(&server_thread, NULL, server_fun_thread, NULL);

    ret = 0;

    return ret;
}

void server_close(void)
{
    close(sock_fd);
}