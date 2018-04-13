#include "mylib.h"
#include "../share/protocol.h"
#include "msg.h"
#include "func.h"
#include "show.h"
#include <pthread.h>
#include <poll.h>

//存储服务器的套接字
extern int server_fd;
extern int my_ufd;
void general_vertify_code(char *code)
{
    char serial_num[3][26] = { 
        {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6'},
        {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'},
        {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'},
        };
    code[0] = serial_num[rand()%3][rand()%26];
    code[1] = serial_num[rand()%3][rand()%26];
    code[2] = serial_num[rand()%3][rand()%26];
    code[3] = serial_num[rand()%3][rand()%26];
}

int is_esc_ch()
{
    if(ch[0] == 27 && ch[1] == 0 && ch[2] == 0) {
        return 1;
    }
    return 0;
}

int is_tab_ch()
{
    if(ch[0] == 9 && ch[1] == 0 && ch[2] == 0) {
        return 1;
    }
    return 0;
}

int is_shift_tab_ch()
{
    if(ch[0] == 27 && ch[1] == 91 && ch[2] == 90) {
        return 1;
    }
    return 0;
}

int is_enter_ch()
{
    if(ch[0] == 13 && ch[1] == 0 && ch[2] == 0) {
        return 1;
    }
    return 0;
}

int is_back_ch()
{
    if(ch[0] == 127 && ch[1] == 0 && ch[2] == 0) {
        return 1;
    }
    return 0;
}

int is_num_ch()
{
    if(ch[0] >= '0' && ch[0] <= '9' && ch[1] == 0 && ch[2] == 0)
        return 1;
    return 0;
}

int is_alpha_ch()
{
    if(ch[0]>='a' && ch[0] <='z' || ch[0] >= 'A' && ch[0] <= 'Z')
        if(ch[1] == 0 && ch[2] == 0)
            return 1;
    return 0;
}

int is_right_ch()
{
    if(ch[0] == 27 && ch[1] == 91 && ch[2] == 67)
        return 1;
    return 0;
}

int is_left_ch()
{
    if(ch[0] == 27 && ch[1] == 91 && ch[2] == 68)
        return 1;
    return 0;
}

int is_up_ch()
{
    if(ch[0] == 27 && ch[1] == 91 && ch[2] == 65)
        return 1;
    return 0;
}

int is_down_ch()
{   
    if(ch[0] == 27 && ch[1] == 91 && ch[2] == 66)
        return 1;
    return 0;
}

int is_dirc_ch()
{
    return is_up_ch() || is_down_ch() || is_left_ch() || is_right_ch();
}

void set_main_curs_pos(enum MAIN_CURS_POS curPos)
{
    int col_friend = FRIEND_COL + 1;
    switch(curPos)
    {
        case MAIN_LINKMAN:
            cur_x = LINKMAN_ROW;
            cur_y = LINKMAN_COL;
            break;
        case MAIN_GROUP:
            cur_x = GROUP_NAME_ROW;
            cur_y = GROUP_NAME_COL;
            break;
        case F1:
            cur_x = FRIEND_ROW;	
            cur_y = col_friend;
            break;
        case F2:
            cur_x = FRIEND_ROW + 1;	
            cur_y = col_friend;
            break;
        case F3:
            cur_x = FRIEND_ROW + 2;	
            cur_y = col_friend;
            break;
        case F4:
            cur_x = FRIEND_ROW + 3;	
            cur_y = col_friend;
            break;
        case F5:
            cur_x = FRIEND_ROW + 4;	
            cur_y = col_friend;
            break;
        case F6:
            cur_x = FRIEND_ROW + 5;	
            cur_y = col_friend;
            break;
        case F7:
            cur_x = FRIEND_ROW + 6;	
            cur_y = col_friend;
            break;
        case F8:
            cur_x = FRIEND_ROW + 7;	
            cur_y = col_friend;
            break;
        case F9:
            cur_x = FRIEND_ROW + 8;	
            cur_y = col_friend;
            break;
        case F0:
            cur_x = FRIEND_ROW + 9;	
            cur_y = col_friend;
            break;
        case  MAIN_FIND:
            cur_x = ADD_ROW;
            cur_y = ADD_COL;
            break;
        case MAIN_RETURN:
            cur_x = MAIN_RETURN_ROW;
            cur_y = MAIN_RETURN_COL;
            break;
        default:
            fprintf(stderr, "set_main_curs_pos error, 程序不应该跑到这里来\n");
            break;
    }
    printf("\e[%d;%dH",cur_x, cur_y);
    fflush(NULL);
}

void deal_find_usr(struct node *msg, enum MAIN_CURS_POS *curs)
{
    struct find_usr_str frd_msg;
    memcpy(&frd_msg, msg->content, msg->cnt*msg->size);
    char tmp[150] = {0};
    if(strcmp(frd_msg.gender, "m")==0 || strcmp(frd_msg.gender, "M")== 0)
        strcpy(frd_msg.gender, "男");
    else 
        strcpy(frd_msg.gender, "女");
    sprintf(tmp, "查询 %d 的结果 |昵称:%s |性别:%s |省份:%s",frd_msg.id, frd_msg.name, frd_msg.gender, frd_msg.addr);
    set_tips(tmp);
    tips_show();
    //询问是否发送加为好友
    printf("\e[%d;%dH\e[0m是否将 %s 加为好友?", ADD_FRIEND_ID_ROW -4, ADD_FRIEND_ID_COL ,frd_msg.name);
    printf("\e[%d;1H\e[35X",ADD_FRIEND_BTN_ROW); 
    printf("\e[%d;%dH\e[1m确认\e[%d;%dH取消\e[0m",ADD_FRIEND_BTN_ROW, ADD_FRIEND_BTN_COL, CANCLE_FRIEND_BTN_ROW, CANCLE_FRIEND_BTN_COL);
    printf("\e[%d;%dH", ADD_FRIEND_ID_ROW, ADD_FRIEND_BTN_COL);
    fflush(NULL);

    enum confirm {YES, NO};
    enum confirm curs_pos = YES;
    while(1) {
        get_str();
        if(is_esc_ch()) {
            cover_add_friend();
            *curs = MAIN_FIND;
            set_tips("已取消添加好友");
            tips_show();
            printf("\e[%d;%dH", ADD_ROW, ADD_COL);
            fflush(NULL);
            return ;
        } else if( is_tab_ch() || is_shift_tab_ch()) {  //按下tab
            if(curs_pos == YES) {
                curs_pos = NO;
                printf("\e[%d;%dH", CANCLE_FRIEND_BTN_ROW, CANCLE_FRIEND_BTN_COL);
            } else {
                curs_pos = YES;
                printf("\e[%d;%dH", ADD_FRIEND_BTN_ROW, ADD_FRIEND_BTN_COL);
            }
        } else if( is_enter_ch() ) {
            if(curs_pos == NO) {
                set_tips("取消添加好友");
                tips_show();
            } else { 
                int dstId = frd_msg.id;
                frd_msg.id = my_id;
                strcpy(frd_msg.name, nickname);
                strcpy(frd_msg.gender, my_gender);
                strcpy(frd_msg.addr, my_addr);
                send_msg(dstId, SERVER, add_usr, 1, sizeof(frd_msg), (char *)&frd_msg);
                set_tips("已发送给服务器确认添加好友消息");
                tips_show();
            }
            cover_add_friend();
            *curs = MAIN_FIND;
            printf("\e[%d;%dH", ADD_ROW, ADD_COL);
            fflush(NULL);
            return ;
        }
        fflush(NULL);
    }
}

void deal_add_usr_confirm(struct node *msg, enum MAIN_CURS_POS *curs_pos)
{
    struct find_usr_str frd_msg;
    memcpy(&frd_msg, msg->content, msg->cnt*msg->size);
    printf("\e[10;10HID %d \e[11;10H名字 %s \e[12;10H省份 %s \e[13;10H添加你为好友(1,同意, 2,不同意) \e[14;15H", 
            frd_msg.id, frd_msg.name, frd_msg.addr);
    fflush(NULL);
    int input;
    scanf("%d", &input);
    cover_add_friend();
    *curs_pos = MAIN_FIND;
    printf("\e[%d;%dH", ADD_ROW, ADD_COL);
    if(input == 1) {
        send_msg(frd_msg.id, SERVER, add_usr_yes, 1, sizeof(frd_msg), (char *)&frd_msg);
    }
    fflush(NULL);
}

void deal_server_msg(struct node *msg, enum MAIN_CURS_POS *curs_pos)
{
    if(msg->msg_type == err) {
        enum err_type err_msg = *(enum err_type *)msg->content;
        switch(err_msg)
        {
            case usr_not_exit:
                err_show("你查找的用户不存在");
                break;
            case usr_not_online:
                set_tips("用户没有在线，不能添加好友");
                tips_show();
                break;
            case is_friend:
                set_tips("你们已经是好友，无需添加");
                tips_show();
                break;
        }
        return ;
    } 

    switch(msg->msg_type) 
    {
        case find_usr:
            deal_find_usr(msg, curs_pos);
            break;
        case add_usr_confirm:
            deal_add_usr_confirm(msg, curs_pos);
            break;
        default:
            break;
    }
}

//功  能:主窗口中移动光标和选择功能
void main_func()
{
    //用于接收服务器或客户端的消息
    struct node msg;
    struct pollfd arr[3];
    arr[0].fd = 0;
    arr[0].events = POLLIN;

    arr[1].fd = server_fd;
    arr[1].events = POLLIN;

    arr[2].fd = my_ufd;
    arr[2].events = POLLIN;
    enum MAIN_CURS_POS curs_pos = MAIN_GROUP;
    while(1) {
        struct termios newattr, oldattr;//屏幕终端码流属性
        tcgetattr(0, &oldattr);  //获取终端属性
        cfmakeraw(&newattr);     //创建新的属性
        tcsetattr(0,TCSANOW,&newattr); //设置新的终端码流属性
        bzero(ch,3);
        poll(arr, 3, -1);
        if(arr[1].revents == arr[1].events) {//服务器来信息
            tcsetattr(0,TCSANOW,&oldattr);
            recv_msg(&msg, server_fd);
            deal_server_msg(&msg, &curs_pos);
            continue;
        }
        if(arr[2].revents == arr[2].events) {//客户端来信息
            tcsetattr(0,TCSANOW,&oldattr);
            recv_msg(&msg, my_ufd);
            continue;
        }
        if(arr[0].revents == arr[0].events) {
            read(0,ch,3);
            tcsetattr(0,TCSANOW,&oldattr);
        }
        err_show("");

        if(is_esc_ch()) { //退出整个程序
            system("clear");
            printf("\e[42;34m已退出KK\n\e[0m");
            exit(0);
        } else if( is_tab_ch() ) {  //按下tab
            if(curs_pos == MAIN_RETURN) {
                curs_pos = MAIN_LINKMAN;
            } else {
                ++curs_pos;
                if(curs_pos == F1 + cnt_frd)
                    curs_pos = MAIN_FIND;
            }
            set_main_curs_pos(curs_pos);
        } else if( is_shift_tab_ch() ) {//按下shift_tab
            if( curs_pos == MAIN_LINKMAN ) {
                curs_pos = MAIN_RETURN;
            } else {
                if(curs_pos == MAIN_FIND) {
                    curs_pos = F1 + cnt_frd;
                }
                --curs_pos;
            }
            set_main_curs_pos(curs_pos);
        } else if( is_enter_ch() ) { //回车
            if(curs_pos == MAIN_RETURN) {
                system("clear");
                printf("\e[42;34m已退出KK\n\e[0m");
                exit(0);
            }
            if(curs_pos == MAIN_FIND) {
                draw_add_friend();
                add_friend_func();
                cover_add_friend();
                curs_pos = MAIN_FIND;
                printf("\e[%d;%dH", ADD_ROW, ADD_COL);
                fflush(NULL);
                continue;
            }
            if(curs_pos >= F1 && curs_pos <= F1+cnt_frd-1) {
                //选中了某个用户名
                printf("\e[10;10H选中了%d个好友", curs_pos - F1 + 1);
                fflush(NULL);
            }
        }
    }
}

int set_login_curs_pos(enum LOGIN_CURS_POS curPos, int cnt_accont, int cnt_psw)
{
    switch(curPos)
    {
        case LOGIN_ID:
            cur_x = LOGIN_ACCOUNT_ROW;
            cur_y = LOGIN_ACCOUNT_COL + cnt_accont;
            break;
        case LOGIN_PSW:
            cur_x = LOGIN_PSW_ROW;
            cur_y = LOGIN_PSW_COL + cnt_psw;
            break;
        case LOGIN_LOGIN:
            cur_x = LOGIN_BTN_ROW;	
            cur_y = LOGIN_BTN_COL;
            break;
        case REG:
            cur_x = LOGIN_REG_ROW;
            cur_y = LOGIN_REG_COL;
            break;
        case LOGOFF:
            cur_x = LOGIN_OFF_ROW;
            cur_y = LOGIN_OFF_COL;
            break;
        case LOGIN_RETURN:
            cur_x = LOGIN_RETURN_ROW;
            cur_y = LOGIN_RETURN_COL;
            break;
        default:
            fprintf(stderr, "set_login_curs_pos error, 程序不应该跑到这里来\n");
            break;
    }
    printf("\e[%d;%dH",cur_x, cur_y);
    fflush(NULL);
}

//功  能:登录界面中移动光标和输入选择
void login_func()
{
    enum LOGIN_CURS_POS curs_pos = LOGIN_ID;
    char usr_accont[ID_LEN+1] = {0}; //存储帐号输入
    int cnt_accont = 0; //记录账户输入多少个

    char psw[PSW_LEN+1] = {0}; //存储密码输入
    int cnt_psw = 0;  //记录密码输入多少个

    while(1) {
        get_str(); //用户输入字符
        err_show("");
        //输入字符的动作
        if(is_esc_ch()) { //退出整个程序
            system("clear");
            printf("\e[42;34m已退出KK\n\e[0m");
            exit(0);
        } else if( is_tab_ch() ) {  //按下tab
            if(curs_pos == LOGIN_RETURN) {
                curs_pos = LOGIN_ID;
            } else {
                ++curs_pos;
            }
            set_login_curs_pos(curs_pos, cnt_accont, cnt_psw);
        } else if( is_shift_tab_ch() ) {//按下shift_tab
            if( curs_pos == LOGIN_ID ) {
                curs_pos = LOGIN_RETURN;
            } else {
                --curs_pos;
            }
            set_login_curs_pos(curs_pos, cnt_accont, cnt_psw);
        } else if( is_enter_ch() ) { //回车
            if(curs_pos == REG) { //选择了注册界面
                disWin = REG_WIN;
                return ;
            } 
            if(curs_pos == LOGOFF) {//选择了注销界面
                disWin = LOGOFF_WIN;
                return ;
            }
            if(curs_pos == LOGIN_RETURN) {
                system("clear");
                printf("\e[42;34m已退出KK\n\e[0m");
                exit(0);
            }
            if( cnt_accont == 0) {
                curs_pos = LOGIN_ID;
                set_login_curs_pos(curs_pos, cnt_accont, cnt_psw);
                err_show("请输入帐号");
                continue;
            }
            if(cnt_psw == 0 ) {
                curs_pos = LOGIN_PSW;
                set_login_curs_pos(curs_pos, cnt_accont, cnt_psw);
                err_show("请输入密码");
                continue;
            }
            //这里开始发送登录报文
            struct login_str log_msg;
            log_msg.id = atoi(usr_accont);
            memcpy(log_msg.psw, psw, sizeof(psw));
            memcpy(&log_msg.udp_addr, &my_udp, sizeof(my_udp));
            send_msg(SERVER_ID, SERVER, login, 1, sizeof(log_msg), (char *)&log_msg);
            struct node msg;
            recv_msg(&msg, server_fd);
            if(msg.msg_type == err) {
                err_show("登录失败，帐户，用户名错误或已经登录");
                continue;
            } 
            set_tips("");
            my_id = log_msg.id;
            strcpy(nickname, ((struct register_usr_str*)msg.content)->name);
            strcpy(my_gender, ((struct register_usr_str*)msg.content)->gender);
            if(strcmp(my_gender, "m") == 0 || strcmp(my_gender, "M")==0 )
                strcpy(my_gender, "男");
            else
                strcpy(my_gender, "女");
            strcpy(my_addr, ((struct register_usr_str*)msg.content)->addr);
            recv_msg(&msg, server_fd);
            memcpy(frd ,msg.content, FRIENDS_MAX * sizeof(struct friends_save));
            disWin = MAIN_WIN;
            return ;
        } else if( is_back_ch()) {
            if(curs_pos == LOGIN_ID) {
                if(cnt_accont == 0)
                    continue;
                usr_accont[--cnt_accont] = '\0';
                printf("\e[1D \e[1D"); //覆盖一个空格
            } else if(curs_pos == LOGIN_PSW) {
                if(cnt_psw==0)
                    continue;
                psw[--cnt_psw] = '\0';
                printf("\e[1D \e[1D"); //覆盖一个空格
            }
            fflush(NULL);
        }else if( is_num_ch()) {//数字
            if(curs_pos == LOGIN_ID) {
                if(cnt_accont == ID_LEN)
                    continue;
                putchar(ch[0]);
                usr_accont[cnt_accont++] = ch[0];
            } else if (curs_pos== LOGIN_PSW) {
                if(cnt_psw == PSW_LEN)
                    continue;
                putchar('*');
                psw[cnt_psw++] = ch[0];
            }
            fflush(NULL);
        } else if(is_alpha_ch()) {//字符
            if(curs_pos == LOGIN_ID) {
                err_show("帐号只能为数字");
                continue;
            } else if (curs_pos == LOGIN_PSW) {
                if(cnt_psw == PSW_LEN)
                    continue;
                putchar('*');
                psw[cnt_psw++] = ch[0];
            }
            fflush(NULL);
        }
    }
}

int set_reg_curs_pos(enum REGISTER_CURS_POS curPos, int cnt_usr_name, int cnt_psw,int cnt_cf_psw,int cnt_gender,
        int cnt_addr,int cnt_code , int cnt_usr_name_cn ,  int cnt_addr_cn)
{
    switch(curPos)
    {
        case REG_NAME:
            cur_x = REG_NAME_ROW;
            cur_y = REG_NAME_COL + 9 + cnt_usr_name - cnt_usr_name_cn; //输入一个中文的时候占据3个字符，而显示只占两个，所以要减去cnt_usr_name_cn
            break;
        case REG_PSW:
            cur_x = REG_PSW_ROW;
            cur_y = REG_PSW_COL + 9 + cnt_psw;
            break;
        case REG_CF_PSW:
            cur_x = REG_CF_PSW_ROW;	
            cur_y = REG_CF_PSW_COL + 9 + cnt_cf_psw;
            break;
        case REG_GENDER:
            cur_x = REG_GENDER_ROW;
            cur_y = REG_GENDER_COL + 9 + cnt_gender;
            break;
        case REG_ADDR:
            cur_x = REG_ADDR_ROW;
            cur_y = REG_ADDR_COL + 9 + cnt_addr - cnt_addr_cn;
            break;
        case REG_VER_CODE:
            cur_x = REG_VERTIFY_ROW;
            cur_y = REG_VERTIFY_COL + 9 + cnt_code;
            break;
        case REG_REG:
            cur_x = REG_NOW_ROW;
            cur_y = REG_NOW_COL;
            break;
        case REG_RETURN:
            cur_x = REG_RETURN_ROW;
            cur_y = REG_RETURN_COL;
            break;
        default:
            fprintf(stderr, "set_reg_curs_pos error, 程序不应该跑到这里来\n");
            break;
    }
    printf("\e[%d;%dH",cur_x, cur_y);
    fflush(NULL);
}

//功  能:注册界面中移动光标和输入选择
void register_func()
{
    enum REGISTER_CURS_POS curs_pos = REG_NAME;
    struct register_usr_str reg_msg; 

    bzero(&reg_msg, sizeof(reg_msg));

    char cf_psw[PSW_LEN+1]    = {0}; //存储确认密码输入
    char code[5]              = {0}; // 存储验证码输入

    int cnt_usr_name          = 0; //存储昵称输入个数
    int cnt_psw               = 0; //存储密码输入个数
    int cnt_cf_psw            = 0; //存储确认密码输入个数
    int cnt_gender            = 0; //存储性别输入个数
    int cnt_addr              = 0; //存储地址输入个数
    int cnt_code              = 0; // 存储验证码输入个数

    int cnt_usr_name_cn = 0;
    int cnt_gender_cn = 0;
    int cnt_addr_cn = 0;

    char vertify_code[5] = "";
    general_vertify_code(vertify_code);
    printf("\e[30;42m\e[%d;%dH  %s\e[0m", REG_VERTIFY_ROW, REG_VERTIFY_COL + 9 + 6, vertify_code);
    printf("\e[%d;%dH", REG_NAME_ROW,  REG_NAME_COL + 9);  
    fflush(NULL);

    while(1) {
        get_str(); //用户输入字符
        err_show(""); //清除提示信息

        //判定输入的字符 
        if(is_esc_ch()) {
            system("clear");
            printf("\e[42;34m已退出KK\n\e[0m");
            exit(0);
        } if( is_tab_ch() ) {  //按下tab
            if(curs_pos == REG_RETURN) {
                curs_pos = REG_NAME;
            } else {
                ++curs_pos;
            }
            set_reg_curs_pos(curs_pos, cnt_usr_name, cnt_psw, cnt_cf_psw,cnt_gender, cnt_addr, cnt_code,
                    cnt_usr_name_cn,  cnt_addr_cn);
        } else if( is_shift_tab_ch() ) {//按下shift_tab
            if( curs_pos == REG_NAME ) {
                curs_pos = REG_RETURN;
            } else {
                --curs_pos;
            }
            set_reg_curs_pos(curs_pos, cnt_usr_name, cnt_psw, cnt_cf_psw,cnt_gender, cnt_addr, cnt_code,
                    cnt_usr_name_cn, cnt_addr_cn);
        } else if( is_enter_ch() ) { //回车
            if(curs_pos == REG_RETURN) {
                disWin = LOGIN_WIN;
                return;
            } 
            int isfalse = 0;
            if(cnt_usr_name == 0) {
                err_show("请输入用户名");
                cur_x = REG_NAME_ROW;
                cur_y = REG_NAME_COL + 9 + cnt_usr_name - cnt_usr_name_cn; 
                curs_pos = REG_NAME;
                isfalse = 1;
            } else if(cnt_psw == 0 && cnt_cf_psw == 0) {
                err_show("请输入密码");
                cur_x = REG_PSW_ROW;
                cur_y = REG_PSW_COL + 9 + cnt_psw;
                curs_pos = REG_PSW;
                isfalse = 1;
            } else if(strcmp(reg_msg.psw, cf_psw) != 0) {
                err_show("两次输入的密码不一致");
                cur_x = REG_CF_PSW_ROW;
                cur_y = REG_CF_PSW_COL + 9 + cnt_cf_psw;
                curs_pos = REG_CF_PSW;
                isfalse = 1;
            } else if(cnt_gender == 0) {
                err_show("请填写性别 M/m/F/f");
                cur_x = REG_GENDER_ROW;
                cur_y = REG_GENDER_COL + 9 + cnt_gender;
                curs_pos = REG_GENDER;
                isfalse = 1;
            } else if(cnt_addr == 0) {
                err_show("请输入地址");
                cur_x = REG_ADDR_ROW;
                cur_y = REG_ADDR_COL + 9 + cnt_addr - cnt_addr_cn;
                curs_pos = REG_ADDR;
                isfalse = 1;
            } else if(strcmp(vertify_code, code) != 0) {
                err_show("验证码有误");
                cur_x = REG_VERTIFY_ROW;
                cur_y = REG_VERTIFY_COL + 9 + cnt_code;
                curs_pos = REG_VER_CODE;
                isfalse = 1;
            }
            if(isfalse) {
                printf("\e[%d;%dH",cur_x, cur_y);
                fflush(NULL);
                continue;
            }
            //信息填写完成，发送注册信息给服务器
            send_msg(SERVER_ID, SERVER, register_user, 1, sizeof(reg_msg), (char *)&reg_msg);
            printf("send regisert \n");
            struct node msg;
            recv_msg(&msg, server_fd);
            if(msg.msg_type == err) {
                err_show("注册失败，请稍后再试");
                continue;
            } else {
                char tmp[150] = {0};
                sprintf(tmp, "%d的帐号注册成功，请牢记你的帐号和密码", ((struct register_usr_str *)msg.content)->id);
                set_tips(tmp);
            }
            disWin = LOGIN_WIN;
            return;
        } else if( is_back_ch()) {
            if( curs_pos == REG_NAME) {
                if(cnt_usr_name == 0 )
                    continue;
                reg_msg.name[--cnt_usr_name] = '\0';
                printf("\e[1D \e[1D"); //覆盖一个空格
                cnt_usr_name_cn = 0;
            } else if(curs_pos == REG_PSW) {
                if(cnt_psw==0)
                    continue;
                reg_msg.psw[--cnt_psw] = '\0';
                printf("\e[1D \e[1D"); //覆盖一个空格
            } else if(curs_pos == REG_CF_PSW) {
                if(cnt_cf_psw==0)
                    continue;
                cf_psw[--cnt_cf_psw] = '\0';
                printf("\e[1D \e[1D"); //覆盖一个空格
            } else if(curs_pos == REG_GENDER) {
                if(cnt_gender == 0)
                    continue;
                cnt_gender = 0;
                bzero(reg_msg.gender, sizeof(reg_msg.gender));
                printf("\e[%d;%dH  \e[2D", REG_GENDER_ROW, REG_GENDER_COL + 9);
            } else if(curs_pos == REG_ADDR) {
                if(cnt_addr == 0)
                    continue;
                reg_msg.addr[--cnt_addr] = '\0';
                printf("\e[1D \e[1D");
                cnt_addr_cn = 0;
            } else if(curs_pos == REG_VER_CODE) { 
                if(cnt_code == 0 )
                    continue;
                code[--cnt_code] = '\0';
                printf("\e[1D \e[1D"); //覆盖一个空格
            }
            fflush(NULL);
        }else if( is_num_ch() || is_alpha_ch()) {//数字或字符
            if(curs_pos == REG_NAME) {
                if(cnt_usr_name >= NAME_LEN) {
                    err_show("昵称已达到最大长度");
                    continue;
                }
                putchar(ch[0]);
                reg_msg.name[cnt_usr_name++] = ch[0];
            } else if (curs_pos == REG_PSW) {
                if(cnt_psw == PSW_LEN) {
                    err_show("密码已经达到最大长度");
                    continue;
                }
                putchar('*');
                reg_msg.psw[cnt_psw++] = ch[0];
            } else if (curs_pos == REG_CF_PSW) {
                if(cnt_cf_psw == PSW_LEN) {
                    err_show("密码已经达到最大长度");
                    continue;
                }
                putchar('*');
                cf_psw[cnt_cf_psw++] = ch[0];
            } else if(curs_pos == REG_GENDER) {
                if(cnt_gender > 0 || ((ch[0]!='M') &&( ch[0]!= 'm') && (ch[0]!='F') && (ch[0]!='f'))) {
                    err_show("性别只能为 M/m/F/f");
                    continue;
                }
                putchar(ch[0]);
                reg_msg.gender[cnt_gender++] = ch[0];
            } else if(curs_pos == REG_ADDR) { 
                if(cnt_addr >= 10) {
                    err_show("籍贯已经达到最大长度");
                    continue;
                }
                putchar(ch[0]);
                reg_msg.addr[cnt_addr++] = ch[0];
            } else if (curs_pos == REG_VER_CODE) {
                if(cnt_code == 4) {
                    err_show("验证码已经达到最大长度");
                    continue;
                }
                putchar(ch[0]);
                code[cnt_code++] = ch[0];
            }
            fflush(NULL);
        } else {  //当作中文处理
            if( is_dirc_ch() ) //过滤到上下左右，不然printf输入上下左右的ASCII会是箭头移动
                continue;
            if(curs_pos == REG_NAME) {
                if(cnt_usr_name + 3 >= NAME_LEN) {
                    err_show("昵称已达到最大长度");
                    continue;
                }
                printf("%s",ch);
                reg_msg.name[cnt_usr_name++] = ch[0];
                reg_msg.name[cnt_usr_name++] = ch[1];
                reg_msg.name[cnt_usr_name++] = ch[2];
                ++cnt_usr_name_cn;
            } else if(curs_pos == REG_ADDR) {
                if(cnt_addr > 10) {
                    err_show("省份已达到最大长度");
                    continue;
                }
                printf("%s",ch);
                reg_msg.addr[cnt_addr++] = ch[0];
                reg_msg.addr[cnt_addr++] = ch[1];
                reg_msg.addr[cnt_addr++] = ch[2];
                ++cnt_addr_cn;
            }
            fflush(NULL);
        }
    }
}

//功  能: 查找好友功能
//0退出查找啊 ， 1已经发送查找信息
int add_friend_func()
{
    char usr_accont[ID_LEN+1] = {0}; //存储帐号输入
    int cnt_accont = 0; //记录账户输入多少个
    printf("\e[43m");
    while(1) {
        get_str(); //用户输入字符
        err_show(""); //清除提示信息

        if(is_esc_ch()) {
            return 0;
        } else if( is_enter_ch() ) { //回车
            //发送查找好友信息给服务器

            struct find_usr_str frd_msg; 
            frd_msg.id = atoi(usr_accont);
            if(frd_msg.id <= KK_ID_START) {
                err_show("查找的ID太小，系统没有可能分配");
                continue;
            }
            if(frd_msg.id == my_id ) {
                err_show("不要查找自己");
                continue;
            }
            send_msg(SERVER_ID, SERVER, find_usr, 1, sizeof(frd_msg), (char *)&frd_msg);
            return 1;
        } else if( is_num_ch()) {//数字
            if(cnt_accont == 4) {
                err_show("已经是ID的最大长度");
                fflush(NULL);
                continue;
            }
            putchar(ch[0]);
            usr_accont[cnt_accont++] = ch[0];
            fflush(NULL);
        } else if(is_alpha_ch()) {//字符
            err_show("字母不能输入");
            fflush(NULL);
            continue;
        } else if( is_back_ch()) {
            if(cnt_accont==0)
                continue;
            usr_accont[--cnt_accont] = '\0';
            printf("\e[1D\e[43m \e[1D"); //覆盖一个空格
            fflush(NULL);
        }
    } 
    return 0;
}
