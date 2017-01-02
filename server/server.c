#include "../share/protocol.h"
#include "func.h"
#include "msg.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>


void signal_int_quit( int signo);

//储存客户端的id和对应的地址
struct sk_port_ip kk_addr[ONLINE_MAX];

//当前用户的连接数
int cnt_usr = 0;

//线程调用函数,客户端链接
void *usr_connect(void *arg);

int main(int argc, char *argv[])
{
    //退出信号处理
    signal(SIGINT, signal_int_quit);
    signal(SIGQUIT, signal_int_quit);

    bzero(kk_addr, sizeof(kk_addr));

    //检查有没有用户帐号信息,如果没有则创建
    if( access("../data/usr.db", F_OK) != 0) {
        FILE *fp = fopen("../data/usr.db", "w");
        int cntKK = 0; //Kk的用户个数
        int kkStart = KK_ID_START; //kk开始帐号
        fwrite(&cntKK , 4, 1, fp);
        fwrite(&kkStart, 4, 1, fp);
        fclose(fp);
    }

    //创建侦听套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1) {
        perror("socket");
        return 0;
    }

    //寻址
    struct sockaddr_in server_addr;
    server_addr.sin_family      = AF_INET;    //这个值为创建套接字时的值
    server_addr.sin_port        = htons(6666);//tcp/ip协议内部存储是按大端字节序,所以需转换
    server_addr.sin_addr.s_addr = INADDR_ANY; //这个宏存储着本机有效地址

    int optval = 1;  //这是一个开关，0表示关，非0开
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int));

    //绑定
    int ret = bind(lfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(ret == -1) {
        perror("bind");
        return 0;
    }

    //侦听
    listen(lfd,2);  //第二个参数是在接收连接请求时，能够允许等待的请求个数
    int cfd;

    struct sockaddr_in client_addr;
    int addr_len = sizeof(client_addr);

    pthread_t tid;
    while(1) {
        //接收连接请求,返回值是一个建立好的通讯套接字
        int cfd = accept(lfd, (struct sockaddr *)(&client_addr), &addr_len);//后面2个参数用来保存连接者的地址，以及地址大小
        if( cfd == -1) {
            fprintf(stderr, "accept error : %s\n", strerror(errno));
            exit(-1);
        }
        pthread_create(&tid, NULL, usr_connect, &cfd);
        pthread_detach(tid); //设置线程为分离状态
    }
    //6.关闭套接字
    close(lfd);
    close(cfd);
    return 0;
}

void *usr_connect(void *arg)
{
    //存储和该用户的链接套接字
    int sk = *(int *)arg;
    unsigned int usr_num = cnt_usr++;
    fprintf(stderr,"套接字 %d 与客户已建立tcp链接\n", sk);
    struct node msg;

    //判断接收到的报文类型
    int ret = 0;
    while(1) {
        ret = recv_msg(&msg, sk); 
        if(ret == -1) {// 线程断开链接
            kk_addr[usr_num].usrId = 0;
            return ;
        }
        switch(msg.msg_type) //第一次链接如果不是这三个类型则丢弃
        {
            case login:    //用户登录
                login_func( &kk_addr[cnt_usr], msg.content, sk, usr_num);
                break; 
            case register_user: //用户注册
                register_user_func(msg.content, sk);
                break;
            case get_friends:
                send_usr_func(msg.src_id,(int *)msg.content, sk);
            case find_usr: //客户端要查找好友
                find_usr_func(msg.src_id, msg.content, sk);
                break;
            case add_usr:
                add_usr_func(msg.dst_id, msg.content, sk);
                break;
            case add_usr_yes:
                add_usr_yes_func(msg.src_id, (struct find_usr_str *)msg.content, sk);
                break;
            default:
                printf("程序不应该跑到这里来\n");
                exit(0);
        }
    }
    printf("退出线程\n");
}

//功  能:信号处理函数,用于退出
void signal_int_quit( int signo)
{
    char ch;
    printf("是否退出服务器Y/N :");
    scanf("%c", &ch);
    getchar();
    if(ch == 'Y' || ch == 'y') {
        printf("服务器已退出\n");
        exit(0);
    }
    else
        printf("服务器正常运行\n %c", ch);
}

