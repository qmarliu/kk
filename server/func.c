#include "../share/protocol.h"
#include "func.h"
#include "msg.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int online_sub(unsigned int id)
{
    int i;
    for(i=0; i<ONLINE_MAX; ++i) {
       if (kk_addr[i].usrId == id) {
           return i;
       }
    }
    return -1;
}

//功  能: 处理用户发送来的注册消息
void register_user_func(char *reg_content, int sk)
{
    struct register_usr_str *usr_reg = (struct register_usr_str *)reg_content;
    printf("name:%s, psw:%s, gender:%s,addr:%s\n",usr_reg->name,
            usr_reg->psw, usr_reg->gender, usr_reg->addr);
    FILE *fp = fopen("../data/usr.db", "r+");
    ERRP(fp == NULL , "fopen", 
      printf("文件操作失败, 发送注册失败信息");return  send_msg(0, USR, err, 0, 0, NULL,sk);
            );

    int cntKK=0,newKK=0;
    fread(&cntKK, 4, 1, fp);
    fread(&newKK, 4, 1, fp);

    //写入新注册的信息
    fseek(fp, 8 + cntKK*sizeof(struct register_usr_str), SEEK_SET);
    usr_reg->id = cntKK++;
    fwrite(usr_reg, sizeof(struct register_usr_str), 1, fp);

    //kk个数+1
    rewind(fp);
    fwrite(&cntKK, 4, 1, fp);

    //kk帐号+1;
    ++newKK;
    usr_reg->id = newKK;
    fwrite(&newKK, 4, 1, fp);
    printf("cntKK:%d , newKK:%d \n",cntKK,newKK);
    fclose(fp);

    //创建以KK名为名的文件
    char kk_id[50] = {0};
    sprintf(kk_id, "../data/%d", newKK);
    fp = fopen(kk_id, "w");
    ERRP(fp == NULL , "fopen", 
      printf("文件创建失败，发送注册失败信息\n");return  send_msg(0, USR, err, 0, 0, NULL,sk);
            );
    fwrite(usr_reg, sizeof(struct register_usr_str), 1, fp);
    
    //十个的好友信息
    struct friends_save f[FRIENDS_MAX];
    bzero(f, sizeof(f));
    /*
     *f[0].id = 1003;
     *strcpy(f[0].name, "测试");
     */
    fwrite(f, sizeof(struct friends_save), FRIENDS_MAX, fp);

    //发送注册成功给用户，并带有KK号码
    printf("发送注册成功信息给用户\n");
    send_msg(0, USR, register_user, 1, sizeof(struct register_usr_str), (char *)usr_reg,sk);
    fclose(fp);
}

//功  能: 处理用户传过来的登录信息
void login_func(struct sk_port_ip *usr_udp, char *reg_content, int sk, int usrNum)
{
    struct login_str *usr_login = (struct login_str *)reg_content;
   
    if(online_sub(usr_login->id) >=0 ) { //说明已经登录
        printf("已登录,用户重新登录");
        send_msg(0, USR, err, 0, 0, NULL,sk);
        return ;
    }
    //查找以登录帐号为名的文件，如果没有，则说明没有这个帐号，登录失败
    char kk_id[50] = {0};
    sprintf(kk_id, "../data/%d", usr_login->id);
    printf("kk_id %s\n",kk_id);
    FILE *fp = fopen(kk_id, "r");
    ERRP(fp == NULL , "fopen", 
            printf("没有这个帐号，登陆失败"); return  send_msg(0, USR, err, 0, 0, NULL,sk);
            );
    struct register_usr_str read_usr;//register_usr_str里能有登录的信息
    fread(&read_usr, sizeof(read_usr), 1, fp);
    extern int cnt_usr;
    printf("sk: %d, id:%d\n ",sk, read_usr.id);
    kk_addr[usrNum].sk = sk;
    kk_addr[usrNum].usrId = read_usr.id;

    //读取密码
    if(strcmp( read_usr.psw, usr_login->psw) != 0) {
        fclose(fp);
        printf("密码错误，登录失败"); 
        return send_msg(0, USR, err, 0, 0, NULL,sk);
    }

    //保存客户端用户通讯的udp地址
    usr_udp->usrId = usr_login->id;
    memcpy( &usr_udp->usr_addr, &usr_login->udp_addr, sizeof(usr_login->udp_addr));
    /*
     *printf("save: sk=%d addr=%s\n", usr_udp->usr_addr.sin_port, inet_ntoa(usr_udp->usr_addr.sin_addr));
     */

    //发送登录成功
    printf("向ID为 %d 的用户发送登录成功\n", usr_login->id);
    //十个好友信息发过去
    struct friends_save friends_info[FRIENDS_MAX];
    fread(friends_info, sizeof(struct friends_save), FRIENDS_MAX, fp);
    send_msg(0, USR, login, 1, sizeof(struct register_usr_str), (char *)&read_usr,sk);
    send_msg(0, USR, login, FRIENDS_MAX, sizeof(struct friends_save), (char *)friends_info,sk);
    fclose(fp);
}

//功  能: 发送好友信息给用户
void send_usr_func(int usrId,int *zuId, int sk)
{
}

//功  能: 查找某个用户的信息
void find_usr_func(int srcId, char *find_content,int sk)
{
    struct find_usr_str* usr = (struct find_usr_str *)find_content;
    char kk_id[50] = {0};
    sprintf(kk_id, "../data/%d", usr->id);
    FILE *fp = fopen(kk_id, "r");
    enum err_type err_msg = usr_not_exit;
    ERRP(fp == NULL , "fopen", 
             printf("查找的用户不存在\n"); return  send_msg(srcId, USR, err, 1, 4 , usr_not_exit,sk);
            );
    struct register_usr_str usr_reg;
    fread(&usr_reg, sizeof(struct register_usr_str), 1, fp);
    strcpy(usr->name, usr_reg.name);
    strcpy(usr->gender,usr_reg.gender);
    strcpy(usr->addr, usr_reg.addr);

    //发送查找到的KK信息
    printf("%d查找%d的用户成功", srcId, usr->id);
    send_msg(srcId, USR, find_usr, 1, sizeof(struct find_usr_str), (char *)usr,sk);
    fflush(NULL);
    fclose(fp);
}


//功  能: 转发
void add_usr_func(int dstId, char *find_content,int sk)
{
    struct find_usr_str* usr = (struct find_usr_str *)find_content;

    //查看是否已经是好友
    enum err_type msg;
    struct friends_save f[FRIENDS_MAX];
    char kk_id[50] = {0};
    sprintf(kk_id, "../data/%d", usr->id);
    FILE *fp = fopen(kk_id, "r+");
    fseek(fp, sizeof(struct register_usr_str), SEEK_SET);
    fread(f, sizeof(struct friends_save), FRIENDS_MAX, fp);
    int i;
    for(i=0; i<FRIENDS_MAX; ++i) {
        if(f[i].id == dstId) {
            printf("已经是好友\n");
            msg = is_friend;
            send_msg(usr->id, USR, err, 1, 4, (char *)&msg,sk);
            return ;
        }
    }

    int sub = online_sub(dstId);
    if(sub != -1) {
        //转发
        send_msg(dstId, USR, add_usr_confirm, 1,
                sizeof(struct find_usr_str), find_content, kk_addr[sub].sk);
    } else {
        msg=usr_not_online;
        //用户不在线
        send_msg(usr->id, USR, err, 1, 4, (char *)&msg,sk);
    }
}

//功  能: 相互添加好友
void  add_usr_yes_func(unsigned int src_id, struct find_usr_str *content, int sk)
{
    char kk_id[50] = {0};
    struct register_usr_str frd_info;
    //确认方添加对方为好友
    sprintf(kk_id, "../data/%d", src_id);
    FILE *fp = fopen(kk_id, "r+");
    fread(&frd_info, sizeof(struct register_usr_str), 1, fp);
    
    //十个的好友信息
    struct friends_save f[FRIENDS_MAX];
    fread(f, sizeof(struct friends_save), FRIENDS_MAX, fp);
    int i=0;
    for(i=0; i<FRIENDS_MAX; ++i) {
        if(f[i].id == 0) {
            f[i].id = content->id;
            strcpy(f[i].name ,content->name);
            strcpy(f[i].addr , content->addr);
            strcpy(f[i].gender, content->gender);
            break;
        }
    }

    fseek(fp, sizeof(struct register_usr_str), SEEK_SET);
    fwrite(f, sizeof(struct friends_save), FRIENDS_MAX, fp);
    fclose(fp);
    //请求方添加好友
    sprintf(kk_id, "../data/%d", content->id);
    fp = fopen(kk_id, "r+");
    fseek(fp, sizeof(struct register_usr_str), SEEK_SET);

    //十个的好友信息
    fread(f, sizeof(struct friends_save), FRIENDS_MAX, fp);
    for(i=0; i<FRIENDS_MAX; ++i) {
        if(f[i].id == 0) {
            f[i].id = frd_info.id;
            strcpy(f[i].name ,frd_info.name);
            strcpy(f[i].addr , frd_info.addr);
            strcpy(f[i].gender, frd_info.gender);
            break;
        }
    }

    fseek(fp, sizeof(struct register_usr_str), SEEK_SET);
    fwrite(f, sizeof(struct friends_save), FRIENDS_MAX, fp);
    fclose(fp);
}
