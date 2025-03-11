#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include "Ai.h"
//主函数
void main(void)
{ 
    WTST = 0;  //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
    EAXFR = 1; //扩展寄存器(XFR)访问使能
    CKCON = 0; //提高访问XRAM速度

    //液晶屏初始化
    LCD_Init();
		Ai_init();
	
    //循环进行各项测试    
    while(1)
    {
        aiTest();
    }
}
