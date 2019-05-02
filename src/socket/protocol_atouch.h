#ifndef __PROTOCOL_ATOUCH_H__
#define __PROTOCOL_ATOUCH_H__

int atouch_receive(char* buf, int len);
int cmd_atouch_creat(char cmd, char *in_buf, int len, char *out_buf);
void atouch_reset(void);

#endif
