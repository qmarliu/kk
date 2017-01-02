#ifndef _FUNC_H_
#define _FUNC_H_

//存储光标的位置
extern int cur_x;
extern int cur_y;

//主界面光标的位置
enum MAIN_CURS_POS {
    MAIN_LINKMAN,  //光标在联系人位置
    MAIN_GROUP,     //光标在群位置
    F1, F2, F3,F4, F5, F6,F7, F8, F9, F0,//光标在十个分组位置
    MAIN_FIND, //光标在查找位置
    MAIN_RETURN,
};

//登录界面光标的位置
enum LOGIN_CURS_POS { 
    LOGIN_ID,   //光标在输入登入帐号位置
    LOGIN_PSW,  //密码位置
    LOGIN_LOGIN, //登录按钮位置
    REG,        //注册位置
    LOGOFF,     //注销位置
    LOGIN_RETURN, //返回
};

//注销界面光标的位置
enum REGISTER_CURS_POS {
    REG_NAME,  //光标在输入昵称位置
    REG_PSW,    //密码位置
    REG_CF_PSW, //密码确认位置
    REG_GENDER, //性别位置
    REG_ADDR,   //地址输入位置
    REG_VER_CODE , //验证码
    REG_REG,   //确认注册
    REG_RETURN, //返回
};

void login_func();
void register_func();
void main_func();
int add_friend_func();
#endif 
