#include "show.h"


//功  能: 右下角显示时间和作者
void draw_author_time()
{
    time_t rawtime;
    struct tm *timeinfo;
    time( &rawtime );
    timeinfo = localtime( &rawtime );
    printf("\e[%d;%dH\e[36m聊天系统 | 作者: 刘磊 | 时间: %d-%02d-%02d %02d:%02d\e[0m",
            AUTHOR_ROW, AUTHOR_COL, 
            1900+timeinfo->tm_year, 1+timeinfo->tm_mon,timeinfo->tm_mday,
            timeinfo->tm_hour,timeinfo->tm_min); 
    fflush(NULL);
}

//功  能: 左上角显示欢迎使用聊天工具信息
void draw_welcome()
{
    printf("\e[%d;%dH\e[36m欢迎使用KK聊天系统v0.1版本，意见或建议发邮件到邮箱liu6uu@163.com\e[0m",
            WELCOME_ROW, WELCOME_COL);

    fflush(NULL);
}

//功  能: 绘制分割线
void draw_devision()
{
    int i;
    printf("\e[%d;%dH\e[30m", AUTHOR_ROW-1, DIVISION_START_COL);   //作者上的分割线
    for(i=0; i<DIVISION_LEN; ++i)
        printf("-");

    printf("\e[%d;%dH\e[30m", WELCOME_ROW+1, DIVISION_START_COL);   //欢迎下的分割线
    for(i=0; i<DIVISION_LEN; ++i)
        printf("-");

    printf("\e[0m");
    fflush(NULL);
}

void draw_picture()
{
    int row=-1;
    printf("\e[%d;%dH", LOGIN_PIC_ROW + (++row), LOGIN_PIC_COL);
    printf("TTTTTTTTTT");
    printf("\e[%d;%dH", LOGIN_PIC_ROW + (++row), LOGIN_PIC_COL);
    printf("|        |");
    printf("\e[%d;%dH", LOGIN_PIC_ROW + (++row), LOGIN_PIC_COL);
    printf("|        |");
    printf("\e[%d;%dH", LOGIN_PIC_ROW + (++row), LOGIN_PIC_COL);
    printf("|        |");
    printf("\e[%d;%dH", LOGIN_PIC_ROW + (++row), LOGIN_PIC_COL);
    printf("----------");
}

//功  能: 绘制登录窗口
void draw_login()
{
    draw_author_time(); 
    draw_welcome();
    draw_devision();

    printf("\e[42m"); //设置背景(绿)
    int i,j;
    for(i=0; i<LOGIN_HIG; ++i) {
        printf("\e[%d;%dH", LOGIN_ROW + i, LOGIN_COL);
        for(j=0; j<LOGIN_WID; ++j) {
            printf(" ");
        }
    }

    printf("\e[37m"); //设置右上角-x为白
    printf("\e[%d;%dHX", LOGIN_ROW, LOGIN_COL + LOGIN_WID-1 );

    printf("\e[30m"); //黑色字体显示如下信息
    printf("\e[%d;%dHKK 沟通你我", LOGIN_ROW, LOGIN_COL);
    printf("\e[%d;%dHKK2014", LOGIN_PSW_ROW+2, LOGIN_PSW_COL + 3);
    draw_picture();

    printf("\e[34m"); //蓝色字体显示如下信息
    printf("\e[%d;%dH注册帐号", LOGIN_REG_ROW,  LOGIN_REG_COL);
    printf("\e[%d;%dH注销帐号", LOGIN_OFF_ROW,  LOGIN_OFF_COL);
    printf("\e[%d;%dH\e[1m登        录", LOGIN_BTN_ROW ,  LOGIN_BTN_COL);


    printf("\e[0m");//恢复终端颜色，用空格模拟帐号和密码框
    printf("\e[%d;%dH", LOGIN_ACCOUNT_ROW,  LOGIN_ACCOUNT_COL);
    printf("                    ");
    printf("\e[%d;%dH", LOGIN_PSW_ROW, LOGIN_PSW_COL);
    printf("                    ");

    printf("\e[30m");//在帐号框下显示输入条

    //绘制完成后，将光标移动到帐号输入
    printf("\e[%d;%dH", LOGIN_ACCOUNT_ROW,  LOGIN_ACCOUNT_COL);

    printf("\e[0m");
    fflush(NULL);
}

//功  能: 绘制注册窗口
void draw_register() 
{
    system("clear");
    draw_author_time();
    draw_welcome();
    draw_devision();

    printf("\e[42m"); //设置背景(绿)
    int i,j;
    for(i=0; i<REG_HIG; ++i) {
        printf("\e[%d;%dH", REG_ROW + i, REG_COL);
        for(j=0; j<REG_WID; ++j) {
            printf(" ");
        }
    }

    printf("\e[37m"); //设置右上角-x为白
    printf("\e[%d;%dHX", LOGIN_ROW, LOGIN_COL + LOGIN_WID-1 );

    printf("\e[30m"); //黑色字体显示如下信息
    printf("\e[%d;%dHKK 有十亿人已注册，你是其中的哪一个", LOGIN_ROW, LOGIN_COL);

    printf("\e[%d;%dH\e[30;42m    昵称 ", REG_NAME_ROW, REG_NAME_COL);
    printf("\e[0m                    ");

    printf("\e[%d;%dH\e[30;42m    密码 ", REG_PSW_ROW, REG_PSW_COL);
    printf("\e[0m                    ");

    printf("\e[%d;%dH\e[30;42m确认密码 ", REG_CF_PSW_ROW, REG_CF_PSW_COL);
    printf("\e[0m                    ");

    printf("\e[%d;%dH\e[30;42m    性别 ", REG_GENDER_ROW, REG_GENDER_COL);
    printf("\e[0m     ");

    printf("\e[%d;%dH\e[30;42m    籍贯 ", REG_ADDR_ROW, REG_ADDR_COL);
    printf("\e[0m            ");

    printf("\e[%d;%dH\e[30;42m  验证码 ", REG_VERTIFY_ROW, REG_VERTIFY_COL);
    printf("\e[0m     ");

    printf("\e[%d;%dH\e[1m\e[34;42m立 即 注 册", REG_NOW_ROW ,  REG_NOW_COL);

    //绘制完成后，将光标移动到昵称输入
    printf("\e[%d;%dH", REG_NAME_ROW,  REG_NAME_COL + 9);
    printf("\e[0m");
    fflush(NULL);
}

//功  能: 绘制主窗口
void draw_main() 
{
    draw_author_time();
    draw_welcome();
    draw_devision();

    printf("\e[42m"); //边框颜色
    int i, j;
    //显示自己信息背景色绘制
    for(i=0; i<MY_INFO_ROW; ++i) {
        printf("\e[%d;%dH",MAIN_ROW + i, MAIN_COL);
        for(j=0; j<MAIN_WID; ++j) {
            printf(" ");
        }
    }

    ++i;
    printf("\e[%d;%dH",MAIN_ROW + i, MAIN_COL);
    for(j=0; j<MAIN_WID; ++j)//联系人/群 下面一行背景 
        printf(" ");
    //绘制最后一行
    for(i=0; i<MAIN_WID; ++i)
        printf("\e[%d;%dH ", MAIN_ROW + MAIN_HIG-1,MAIN_COL + i );
    //绘制倒数第三行
    for(i=0; i<MAIN_WID; ++i)
        printf("\e[%d;%dH ", MAIN_ROW + MAIN_HIG-3,MAIN_COL + i );


    //绘制左右边框
    for(i=0; i<MAIN_HIG-MY_INFO_ROW -1; ++i) {
        printf("\e[%d;%dH \e[%d;%dH ", MAIN_ROW+MY_INFO_ROW+i, MAIN_COL, 
                MAIN_ROW+MY_INFO_ROW + i, MAIN_COL+MAIN_WID -1);
    }

    //绘制图片
    int pic_w=3, pic_h = 5;
    for(i=0;i<pic_w;++i) {
        for(j=0; j<pic_h; ++j) {
            printf("\e[%d;%dH|", NICKNAME_ROW + i , NICKNAME_COL-6+j);
        }
    }

    printf(" 不能编辑个性签名");

    printf("\e[37m"); //设置右上角x为白
    printf("\e[%d;%dHX", MAIN_ROW, MAIN_COL+MAIN_WID-1 );

    printf("\e[30m"); //黑色字体显示如下信息
    printf("\e[%d;%dHKK", MAIN_ROW, MAIN_COL);

    printf("\e[%d;%dH%s  %s", NICKNAME_ROW+1, NICKNAME_COL, my_gender, my_addr);
    printf("\e[%d;%dH在线 \e[1m%s", NICKNAME_ROW, NICKNAME_COL, nickname);
    printf("\e[0m");
    printf("\e[%d;%dH联系人       |\e[%d;%dH群", LINKMAN_ROW, LINKMAN_COL, GROUP_NAME_ROW, GROUP_NAME_COL);
    printf("\e[%d;%dH添加好友", ADD_ROW, ADD_COL);

    //绘制好友名字
    for(i=0; i<FRIENDS_MAX; ++i) {
        if(frd[i].id != 0) {
            printf("\e[%d;%dH [%u] %s", FRIEND_ROW + i, FRIEND_COL,frd[i].id, frd[i].name);
            ++cnt_frd;
        } else {
            break;
        }
    }

    //将光标定位在群
    printf("\e[%d;%dH", GROUP_NAME_ROW, GROUP_NAME_COL);

    fflush(NULL);
}

//绘制添加好友窗口
void draw_add_friend()
{
    printf("\e[42m"); //边框颜色
    int i;
    //绘制边框
    for(i=0; i<ADD_FRIEND_WID; ++i) {
        printf("\e[%d;%dH ",ADD_FRIEND_ROW, ADD_FRIEND_COL+i);
    }
    for(i=0; i<ADD_FRIEND_WID; ++i) {
        printf("\e[%d;%dH ",ADD_FRIEND_ROW + ADD_FRIEND_HIG-1, ADD_FRIEND_COL+i);
    }
    printf("\e[%d;%dH\e[0mID查找 ", ADD_FRIEND_ID_ROW , ADD_FRIEND_ID_COL);
    printf("\e[43m    ");

    printf("\e[0m\e[%d;%dH", ADD_FRIEND_ID_ROW, ADD_FRIEND_ID_COL + 7);
    fflush(NULL);
}

//窗口擦除去
void cover_add_friend()
{
    //绘制边框
    /*
     *printf("\e[%d;%dH\e[35X",ADD_FRIEND_ROW-2, ADD_FRIEND_COL);
     *printf("\e[%d;%dH\e[35X",ADD_FRIEND_ROW, ADD_FRIEND_COL);
     *printf("\e[%d;%dH\e[35X",ADD_FRIEND_ROW + ADD_FRIEND_HIG-1, ADD_FRIEND_COL);
     *printf("\e[%d;%dH\e[35X", ADD_FRIEND_ID_ROW , ADD_FRIEND_ID_COL);
     */
    int i=-3;
    printf("\e[%d;%dH\e[35X",ADD_FRIEND_ROW+i++, ADD_FRIEND_COL);
    printf("\e[%d;%dH\e[35X",ADD_FRIEND_ROW+i++, ADD_FRIEND_COL);
    printf("\e[%d;%dH\e[35X",ADD_FRIEND_ROW+i++, ADD_FRIEND_COL);
    printf("\e[%d;%dH\e[35X",ADD_FRIEND_ROW+i++, ADD_FRIEND_COL);
    printf("\e[%d;%dH\e[35X",ADD_FRIEND_ROW+i++, ADD_FRIEND_COL);
    printf("\e[%d;%dH\e[35X",ADD_FRIEND_ROW+i++, ADD_FRIEND_COL);
    printf("\e[%d;%dH\e[35X",ADD_FRIEND_ROW+i++, ADD_FRIEND_COL);
    printf("\e[%d;%dH\e[35X",ADD_FRIEND_ROW+i++, ADD_FRIEND_COL);
    printf("\e[%d;%dH\e[35X",ADD_FRIEND_ROW+i++, ADD_FRIEND_COL);
    printf("\e[%d;%dH\e[35X",ADD_FRIEND_ROW+i++, ADD_FRIEND_COL);
    printf("\e[%d;%dH\e[35X",ADD_FRIEND_ROW+i++, ADD_FRIEND_COL);
    fflush(NULL);
}

void set_tips(const char *str)
{
    strcpy(tips, str);
}

void tips_show()
{
    printf("\e[s"); //保存光标位置
    if(strcmp(tips , "") == 0) { //传进来的如果是空字符，则清除之前的提示信息
        printf("\e[0m\e[%d;%dH\e[K", TIPS_ROW, TIPS_COL);
    } else {
        printf("\e[%d;%dH\e[44;30m\e[K%s\e[0m", TIPS_ROW, TIPS_COL, tips);
    }
    printf("\e[u"); //恢复光标位置
    fflush(NULL);
}

void err_show(const char *err)
{
    printf("\e[s"); //保存光标位置
    if(strcmp(err , "") == 0) { //传进来的如果是空字符，则清除之前的提示信息
        printf("\e[0m\e[%d;%dH\e[K", ERR_ROW, ERR_COL);
    } else {
        printf("\e[%d;%dH\e[43;30m\e[K%s\e[0m", ERR_ROW, ERR_COL, err);
    }
    printf("\e[u"); //回复光标位置
    fflush(NULL);
}
