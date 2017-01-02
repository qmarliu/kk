#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include "lib.h"
#include <time.h>

//声明对齐方式，以一个字节对齐，传输过程节省空间
#pragma pack(1) 

//消息类型
enum dst_type { SERVER, USR, GROUP };

//存储客户端的套接字和IP地址，以及对改客户端对应的套接字描述符建立链接
struct sk_port_ip {
    unsigned int sk;
    unsigned int usrId;
    struct sockaddr_in usr_addr;
};

//存储kk账户登录的端口和地址
extern struct sk_port_ip kk_addr[ONLINE_MAX];

//消息类型
enum func_type {
    err,

    find_usr,                   //查找某个好友
    add_usr,                    //添加好友

    get_friends,            //获取某列好友信息

    register_user,               //注册
    login,                       //登陆上线
    logout,                      //登出下线
    chat_msg,                    //发送聊天信息

    friend_login,                //好友上线
    friend_logout,               //好友下线

    add_usr_confirm,    //添加确认
    add_usr_yes,    //同意求情
    add_usr_no,     //不同意
};

enum err_type {
    usr_not_exit,
    usr_not_online,
    is_friend,
};

//协议框架
struct node {
    unsigned int	dst_id;              //目标用户ID
    enum dst_type	dst_type;            //目标类型(群，服务器，用户)
    unsigned int	src_id;              //源用户ID
    enum func_type	msg_type;            //消息类型
    unsigned int	cnt;            //参数个数
    unsigned short	size;           //类型单位大小
    char			content[CONTENT_LEN];//参数内容
};

//FS(front size)，结构体struct node中 content前面的大小
#define FS_LEN sizeof(struct node) - sizeof(char)*CONTENT_LEN

//登录
struct login_str {
    unsigned int id;
    char psw[PSW_LEN+1];
    struct sockaddr_in udp_addr;
};

//好友存储
struct friends_save {
    unsigned int id;
    char name[NAME_LEN + 1];
    char gender[4];
    char addr[ADDR_LEN];
};


//查找好友
struct find_usr_str {
    unsigned int id;
    char name[NAME_LEN+1];
    char gender[4];
    char addr[ADDR_LEN+1];
};

//注册
struct register_usr_str {
    unsigned int id;
    char name[NAME_LEN+1];
    char psw[PSW_LEN+1];
    char gender[4];
    char addr[ADDR_LEN+1];
};
#endif
