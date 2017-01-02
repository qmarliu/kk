#ifndef _LIB_H_
#define _LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <strings.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
/*******************************参数内容大小宏***********************/

#define SERVER_ID -1

//协议框架结构体中，某个类型的最大空间
#define CONTENT_LEN 6*1024

//id最大数值
#define ID_LEN 10

//省最大值
#define ADDR_LEN 16

//好友上线数量
#define FRIENDS_MAX 10

//最大的连接数量
#define ONLINE_MAX 1000

//名字最大长度 15表示五个中文字符，15个英文字符。
#define NAME_LEN 22

//密码最大长度 
#define PSW_LEN 16

//kk帐号开始ID
#define KK_ID_START 1000

//容错宏
#define ERRP(con, func, ret) \
	do { \
		if(con) {     \
			perror(func); \
			ret; \
		}      \
	} while(0)

#endif
