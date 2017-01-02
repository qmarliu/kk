#include "mylib.h"

void get_str()
{
    struct termios newattr, oldattr;//屏幕终端码流属性
	tcgetattr(0, &oldattr);  //获取终端属性
	cfmakeraw(&newattr);     //创建新的属性

    tcsetattr(0,TCSANOW,&newattr); //设置新的终端码流属性

    bzero(ch,3);
    read(0, ch ,3);
//    if(ch[0] == 27 && ch[1] == 0 && ch[2] == 0) {
//        tcsetattr(0,TCSANOW,&oldattr);
//        system("clear");
//        exit(0);
//    }
    tcsetattr(0,TCSANOW,&oldattr);
}
