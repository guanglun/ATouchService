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

#define BUF_SIZE    2048
#define PORT        1989
char server_loop = 1;
int sock_fd;
pthread_t server_thread,client_thread;

void *client_fun_thread(void *arg)
{
    int ret = -1;
    int conn_fd = *(int *)arg;
    char buff[BUF_SIZE];
    char client_loop = 1;

    printf("Client Connect %d\r\n",conn_fd);

    while (client_loop)
    {
        // 接受数据
        ret = recv(conn_fd, buff, BUF_SIZE, 0);
        if (ret <= 0)
        {
            // 客户端关闭
            //printf("client close\n");
            close(conn_fd);
            client_loop = 0;
        }
        else
        {

            // // 添加结束符
            // if (ret < BUF_SIZE)
            // {
            //     memset(&buff[ret], '\0', 1);
            // }
            // printf("recv msg from client: %s\n", buff);

            // // 发送数据
            // send(conn_fd, "Hello", 6, 0);
        }
    }

    printf("Client Disconnect\r\n");
}

void *server_fun_thread(void *arg)
{
    int ret = -1;
    int conn_fd;

	while(server_loop) {
        
		// 接受请求
		if ((conn_fd = accept(sock_fd, (struct sockaddr *)NULL, NULL)) == -1) {
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
	
	int ret = -1;
	

	// 创建socket描述符
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Socket Creat Fail\r\n");
		return ret;
	}

	// 填充sockaddr_in结构
	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	// 绑定sock_fd描述符
	if (bind(sock_fd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1) {
        printf("Bind Fail\r\n");
		return ret;
	}

	// 监听sock_fd描述符
	if(listen(sock_fd, 5) == -1) {
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