/*---------------------------------------------------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* 液晶屏驱动程序参考wiki技术网站提供的开源源码，仅供学习使用          */
/*---------------------------------------------------------------------*/

//=========================================电源接线================================================//
//VDD接DC 3.3V电源
//GND接地
//=======================================液晶屏数据线接线==========================================//
//本模块数据总线类型为8位并口
// LCD_D0~D7     接       P20~P27        //液晶屏8位数据线
//=======================================液晶屏控制线接线==========================================//
// LCD_RS        接        P45           //数据/命令选择控制信号
// LCD_RD        接        P37           //读控制信号
// LCD_RST       接        P47           //复位信号
// LCD_WR        接        P36           //写控制信号
// LCD_CS        接        P05/P53       //片选控制信号
//================================================================================================//
//不使用触摸或者模块本身不带触摸，则可不连接
//触摸屏使用的数据总线类型为SPI
//  T_CS         接        P15          //触摸屏片选控制信号
//  T_CLK        接        P32          //触摸屏SPI时钟信号
//  T_DO         接        P33          //触摸屏SPI读信号
//  T_DIN        接        P34          //触摸屏SPI写信号
//  T_IRQ        接        P14          //触摸屏响应检查信号
//================================================================================================//
// 本实验使用的2.4寸320*240的并口彩屏，由冲哥淘宝店提供：http://e.tb.cn/h.gIlbVqAOj8YXlwo?tk=vFIr3RGTy2n
//**************************************************************************************************/

#include "sys.h"
void delay_ms(int count)  // /* X1ms */
{
     u16 i;
     do{
          i = MAIN_Fosc / 6000;
          while(--i);
     }while(--count);
}

void delay_us(int count)  // /* X1us */
{
        int i,j;
        for(i=0;i<count;i++)
                for(j=0;j<1;j++);
}