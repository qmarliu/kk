#ifndef _MSG_H_
#define _MSG_H_
#include "mylib.h"
#include "../share/protocol.h"

void send_msg( unsigned int dst_id, enum dst_type dst_type, 
        enum func_type msg_type, unsigned int cnt, unsigned short size, char *content);

int recv_msg(struct node *msg,int sk);

#endif
