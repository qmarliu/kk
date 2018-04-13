#ifndef _MYLIB_H
#define _MYLIB_H

#include "../share/lib.h"
#include "../share/protocol.h"
#include <sys/socket.h>
#include <unistd.h>

extern char tips[150];
extern unsigned int my_id;

//存储终端的大小
extern struct winsize WINSIZE;

//客户端自己用于接收Udp的套接字
extern struct sockaddr_in my_udp;

extern struct friends_save frd[FRIENDS_MAX];
extern int cnt_frd;
//我的昵称
extern char nickname[NAME_LEN + 1];
extern char my_gender[4];
extern char my_addr[ADDR_LEN + 1];

//存储光标的位置
extern int cur_x;
extern int cur_y;

//用户输入的字符
extern char ch[4];

//表明当前页面
enum WINDOWS {
    LOGIN_WIN,  //登陆界面
    REG_WIN,    //注册界面
    LOGOFF_WIN, //注销界面
    ADD_FRIEND_WIN, //添加好友界面
    MAIN_WIN,
}disWin;

/*******************************界面宏***********************/

//显示右下角落的作者和时间位置
#define AUTHOR_ROW WINSIZE.ws_row-1
#define AUTHOR_COL WINSIZE.ws_col-53

//显示左上角的欢迎信息
#define WELCOME_ROW 1
#define WELCOME_COL 6

//错误信息显示位置
#define ERR_ROW AUTHOR_ROW-2
#define ERR_COL WELCOME_COL

//上次操作提示消息
#define TIPS_ROW WELCOME_ROW + 2
#define TIPS_COL WELCOME_COL

//分割线的长度
#define DIVISION_START_COL 6
#define DIVISION_LEN WINSIZE.ws_col-2*DIVISION_START_COL

                        /**********主窗口**********/
//主窗口大小
#define MAIN_WID 30
#define MAIN_HIG ERR_ROW-TIPS_ROW - 2 - 2 - 3

//偏离终端右边的距离
#define WIN_OFFSET DIVISION_START_COL + 2

//主窗口的起始位置
#define MAIN_ROW TIPS_ROW + 2
#define MAIN_COL WINSIZE.ws_col - MAIN_WID - WIN_OFFSET

//x按钮的位置
#define MAIN_RETURN_ROW MAIN_ROW
#define MAIN_RETURN_COL MAIN_COL + MAIN_WID -1

//显示自己信息（图片和名字）行数
#define MY_INFO_ROW 6

//昵称显示位置
#define NICKNAME_ROW MAIN_ROW + 2
#define NICKNAME_COL MAIN_COL + 9

//联系人显示位置
#define LINKMAN_ROW MAIN_ROW + MY_INFO_ROW
#define LINKMAN_COL MAIN_COL + 3 

//添加查找的位置
#define ADD_ROW MAIN_ROW + MAIN_HIG-1-1
#define ADD_COL MAIN_COL + 2

//群显示位置
#define GROUP_NAME_ROW LINKMAN_ROW
#define GROUP_NAME_COL MAIN_COL + MAIN_WID -1 - 4

//好友开始显示位置
#define FRIEND_ROW LINKMAN_ROW + 2
#define FRIEND_COL MAIN_COL + 1

                        /**********添加好友窗口**********/
//添加好友窗口大小
#define ADD_FRIEND_WID 30
#define ADD_FRIEND_HIG 5

//窗口的起始位置
#define ADD_FRIEND_ROW WINSIZE.ws_row/2 - ADD_FRIEND_HIG/2
#define ADD_FRIEND_COL DIVISION_START_COL + 2

//ID查找位置
#define ADD_FRIEND_ID_ROW ADD_FRIEND_ROW + 2
#define ADD_FRIEND_ID_COL ADD_FRIEND_COL + 2 

//确认加为好友按钮位置
#define ADD_FRIEND_BTN_ROW ADD_FRIEND_ID_ROW
#define ADD_FRIEND_BTN_COL ADD_FRIEND_ID_COL + 2

//取消加为好友按钮位置
#define CANCLE_FRIEND_BTN_ROW ADD_FRIEND_ID_ROW
#define CANCLE_FRIEND_BTN_COL ADD_FRIEND_ID_COL + 8


                        /**********登录窗口**********/
//登录窗口大小
#define LOGIN_WID 46
#define LOGIN_HIG 16

//登录窗口的启示位置
#define LOGIN_ROW WINSIZE.ws_row/2-LOGIN_HIG/2
#define LOGIN_COL WINSIZE.ws_col/2-LOGIN_WID/2

//字符画的位置
#define LOGIN_PIC_ROW LOGIN_ROW+LOGIN_HIG-9
#define LOGIN_PIC_COL LOGIN_COL+2

//x按钮的位置
#define LOGIN_RETURN_ROW LOGIN_ROW
#define LOGIN_RETURN_COL LOGIN_COL + LOGIN_WID -1

//帐号输入
#define LOGIN_ACCOUNT_ROW LOGIN_PIC_ROW
#define LOGIN_ACCOUNT_COL LOGIN_PIC_COL+10+2 //10字符画的宽度，间距

//密码输入
#define LOGIN_PSW_ROW LOGIN_ACCOUNT_ROW+2
#define LOGIN_PSW_COL LOGIN_ACCOUNT_COL

//注册帐号位置
#define LOGIN_REG_ROW LOGIN_ACCOUNT_ROW
#define LOGIN_REG_COL LOGIN_ACCOUNT_COL+20+2

//注销帐号位置
#define LOGIN_OFF_ROW LOGIN_PSW_ROW
#define LOGIN_OFF_COL LOGIN_PSW_COL+20+2

//登录按钮的位置
#define LOGIN_BTN_ROW LOGIN_ROW+LOGIN_HIG-2
#define LOGIN_BTN_COL LOGIN_COL+10+2+3

                        /**********注册窗口**********/
//注册窗口的大小
#define REG_WID LOGIN_WID
#define REG_HIG LOGIN_HIG + 1

//注册窗口的启示位置
#define REG_ROW LOGIN_ROW
#define REG_COL LOGIN_COL

//x按钮的位置
#define REG_RETURN_ROW REG_ROW
#define REG_RETURN_COL REG_COL + REG_WID -1

//昵称显示位置
#define REG_NAME_ROW REG_ROW + 3
#define REG_NAME_COL REG_COL + 5

//密码显示位置
#define REG_PSW_ROW REG_NAME_ROW + 2
#define REG_PSW_COL REG_NAME_COL

//确认密码显示位置
#define REG_CF_PSW_ROW REG_PSW_ROW + 2
#define REG_CF_PSW_COL REG_PSW_COL

//性别
#define REG_GENDER_ROW REG_CF_PSW_ROW + 2 
#define REG_GENDER_COL REG_CF_PSW_COL

//住址
#define REG_ADDR_ROW REG_GENDER_ROW + 2 
#define REG_ADDR_COL REG_GENDER_COL

//验证码
#define REG_VERTIFY_ROW REG_ADDR_ROW+ 2
#define REG_VERTIFY_COL REG_ADDR_COL

//立即注册
#define REG_NOW_ROW REG_VERTIFY_ROW + 2
#define REG_NOW_COL REG_VERTIFY_COL + 12


/*******************************函数声明**************************/
void get_str();

#endif
