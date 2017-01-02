#include "../share/protocol.h"
#include "mylib.h"
#include "show.h"
#include "func.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <termios.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <netdb.h>

char ch[4];   //接收输入的字符
char tips[150];//上个界面的提示消息

char nickname[NAME_LEN + 1];//昵称
char my_gender[4];//性别
char my_addr[ADDR_LEN+1];//地址


//记录当前光标的位置
int cur_x; 
int cur_y; 

struct winsize WINSIZE;

//我的ID
unsigned int my_id = 0;

//服务器
struct sockaddr_in server_addr;
struct sockaddr_in my_udp;

//十个好友的信息
struct friends_save frd[FRIENDS_MAX];
int cnt_frd;

//我的用于udp接收其他客户端的消息套接字
int my_ufd =0;

//和服务器通讯的套接字  
int server_fd = 0;

//这个函数是终端大小改变产生的信号捕捉处理函数
void sig_winch(int signo);
//初始化全局变量
void init_data();

int main(int argc, char *argv[])
{
    srand(time(NULL)); //获取随机数种子

    //初始化数据
    init_data();
	//获取终端大小
	ioctl(0,TIOCGWINSZ,&WINSIZE);

    //创建通讯套接字
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1) {
        perror("socket");
        return 0;
    }

    //寻址,服务器的地址
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(6666);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //连接
    int ret = connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(ret == -1) {
        perror("connect");
        printf("请检测服务器是否开启\n");
        return 0;
    }

    //下面代码处理客户端与客户端通讯
    char buf[30];
    struct hostent *host;
    gethostname(buf, 30); //获取主机名
    host=gethostbyname(buf);//根据主机名找到hostend结构体，里面有本地IP地址
    
    //我的地址，用于udp通讯
    my_udp.sin_family = AF_INET;
    my_udp.sin_addr = *((struct in_addr *)host->h_addr);

	my_ufd = socket(AF_INET,SOCK_DGRAM,0);

	ret = -1;
	while(ret == -1) { //获取本地的一个端口，若绑定失败，继续寻找端口，直到绑定成功
	    my_udp.sin_port = htons(6001 + rand()%12000);
        ret = bind(my_ufd,(struct sockaddr *)&my_udp,sizeof(my_udp));
        /*
		 *perror("bind");
         */
	}

    disWin = LOGIN_WIN; //首先进入登录界面
    while(1) {
        system("clear");
        tips_show(); 
        switch(disWin) {
            case LOGIN_WIN: //登录
                draw_login();
                login_func();//处理用户在登录界面的选择，函数里会设置全局变量disWin,来控制接下来进入哪个界面
                break;
            case REG_WIN:   //注册
                draw_register();
                register_func();
                break;
            case MAIN_WIN: //主界面
                 draw_main();
                main_func();
                exit(0);
            case LOGOFF_WIN:
                break;
            default:
                printf("disWin, 程序不应该跑到这里来");
                fflush(NULL);
                exit(0);
        }
    }

    //4.关闭套接字
    close(my_ufd);
     return 0;
}

void init_data()
{
    bzero(nickname, sizeof(nickname));
    bzero(my_addr, sizeof(my_addr));
    bzero(my_gender, sizeof(my_gender));
    bzero(tips, sizeof(tips));
    bzero(ch, sizeof(ch));
    bzero(frd, sizeof(frd));
    cnt_frd = 0;
    cur_x = 1;
    cur_y = 1;
    my_id = 0;
}

