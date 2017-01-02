#include "msg.h"
#include "show.h"
#include <sys/types.h>   
#include <sys/socket.h>   

//发送信息给指定的用户或服务器

//存储我与客户端的套接字
extern unsigned int my_id;

//存储服务器的套接字
extern int server_fd;

void send_msg( unsigned int dst_id, enum dst_type dst_type, 
        enum func_type msg_type, unsigned int cnt, unsigned short size, char *content)
{
    struct node msg;
    msg.dst_id = dst_id;
    msg.dst_type = dst_type;
    msg.src_id = my_id;
    msg.msg_type = msg_type;
    msg.cnt = cnt;
    msg.size = size;
    int cnt_size = cnt * size;
    memmove(msg.content, content, cnt_size);


    if( dst_type == SERVER) {
        ERRP( send(server_fd, &msg, cnt_size+ FS_LEN,0) == -1,
            "send", return ); 
        /*
         *printf("msg.act_type : %d\n", msg.act_type);
         *getchar();
         */
    } else if(dst_type == USR) {
    
    } else if(dst_type == GROUP) {
    } else {
        err_show("send_msg,程序不应该运行到这里来\n");
    }
}

//功能:      接收整个结构体。
//参数msg:      用于接收的结构体指针    
//参数sk:     用于接收的套接字
//返回值:     若成功接收,则返回0,否则返回-1;
int recv_msg(struct node *msg,int sk)
{
    int sum = 0;
    int size= 0;
    int content_len = 0;
    bzero(msg, sizeof(msg));
    do { //接收前面的大小
        size = recv(sk, (char *)msg+sum, FS_LEN - sum, 0);
        if(size == 0 ) {
            exit(0);
        }
        ERRP(size < 0, "recv", return -1);
        sum += size;
    } while(sum < FS_LEN);

    /*
     *printf("\n\n源ID<%d>, 目标ID<%d>,时间<%s>, 类型<%d>, 个数<%d>\n" 
     *        ,msg->src_id ,msg->dst_id
     *        ,ctime(&msg->msg_time) , msg->cnt_type
     *        ,msg->unit_size);
     */

    if(msg->cnt == 0 || msg->size == 0) {
        /*
         *printf("接收了一个结构体,总大小%d\n",sum);
         */
        return 0;
    }

    content_len = (msg->size)*(msg->cnt);
    sum = 0;
    do {
        size = recv(sk, (char *)msg+FS_LEN+sum, content_len -sum, 0);
        if(size == 0 ) {
            exit(0);
        }
        ERRP(size < 0, "recv", return -1);
        sum += size;
    } while(sum < content_len);
    /*
     *printf("接收了一个结构体 总大小:%d\n",FS_LEN + content_len);
     */
    return 0;
}
