#ifndef __SERVER_H__
#define __SERVER_H__

int server_init(void);
void server_close(void);

void socket_atouch_send(char *buf, int len);
void socket_adb_send(char *buf, int len);

#endif

