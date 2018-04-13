#ifndef _FUNC_H_
#define _FUNC_H_

void register_user_func(char *, int);
void login_func(struct sk_port_ip *,char *reg_content, int sk, int);
void send_zu_func(int , int);
void find_usr_func(int ,char *find_content,int sk);
void add_usr_func(int usrId, char *find_content,int sk);
void send_usr_func(int usrId,int *, int sk);
void add_usr_yes_func(unsigned int src_id, struct find_usr_str *content, int sk);
#endif
