#include <stdio.h>
#include "lcd.h"
#include "string.h"
#include "sys.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include "stdint.h"
#include "Ai.h"
#include "expression.h"
#include "canvas.h"

//========================variable==========================//
u16 ColorTab[5]={RED,GREEN,BLUE,YELLOW,BRED};//定义颜色数组
u16 ColornTab[8]={RED,MAGENTA,GREEN,DARKBLUE,BLUE,BLACK,LIGHTGREEN};
//=====================end of variable======================//


void DrawTestPage(u8 *str)
{
    //绘制固定栏up
    LCD_Clear(WHITE);
    LCD_Fill(0,0,lcddev.width,20,BLUE);
    //绘制固定栏down
    LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
    POINT_COLOR=WHITE;
    Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//居中显示
    Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"AI calculator",16,1);//居中显示
    //绘制测试区域
    //LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
}

void paddingString(uint8_t *buffer, char c, uint8_t new_length)
{
	uint8_t current_length;
	uint8_t i;

	current_length = (uint8_t)strlen((char *)buffer); // 获取当前字符串长度

	// 如果当前长度已经大于或等于新长度，则无需填充
	if (current_length >= new_length)
	{
		return;
	}

	// 从当前字符串末尾开始填充，直到达到新长度
	for (i = current_length; i < new_length; ++i)
	{
		buffer[i] = c;
	}

	// 添加字符串终止符
	buffer[new_length] = '\0';
}


uint8_t xdata expression[16] = {0};
uint8_t xdata expression_n = 0;
static uint8_t xdata strBuffer[50] = {0};

void countResult(void){
	{
				uint32_t x, y;
				uint8_t sum;
				int in_char = 0;		// 标记当前是否在字符区域
				int char_count = 0;		// 字符数量计数
				uint32_t start_col = 0; // 字符起始列
				expression_n = 0;

				for (x = 0; x < CANVAS_WIDTH; x++)
				{
					sum = 0;
					for (y = 0; y < CANVAS_HEIGHT; y++)
					{
						sum |= canvas[y * CANVAS_WIDTH + x];
					}

					if (sum)
					{
						// 当前列有非零值，表明在字符区域内
						if (!in_char)
						{
							in_char = 1;   // 标记进入字符区域
							start_col = x; // 记录字符起始列
							char_count++;  // 字符数量增加
						}
					}
					else
					{
						// 当前列全为零，表明不在字符区域内
						if (in_char)
						{
							uint32_t end_col;
							in_char = 0; // 标记离开字符区域
							end_col = x - 1;

							if (canvas_process_character(start_col, end_col))
							{
								expression[expression_n++] = Ai_run();
							}
						}
					}
				}

				// 处理最后一个字符到图片末尾的情况
				if (in_char)
				{
					// 最后一个字符的结束列为图片末尾
					uint32_t end_col = CANVAS_WIDTH - 1;

					if (canvas_process_character(start_col, end_col))
					{
						expression[expression_n++] = Ai_run();
					}
					// printf("字符 %d ", char_count);
					// printf("起始列: %d,", start_col);
					// printf("结束列: %d\n", end_col);
				}

				sprintf(strBuffer, "result: %.2f", expression_calc(expression, expression_n));
				// 用在字符后面拼接空格的方法，清理残留字符，避免全屏刷新造成的闪烁。
				paddingString(strBuffer, ' ', 20);
				LCD_ShowString(10,150,16,strBuffer,1);
				
				expression_to_string(expression, expression_n, strBuffer);
				paddingString(strBuffer, ' ', 20);
				
				LCD_ShowString(10,120,16,strBuffer,1);
				
				clean_canvas();
			}
}


void aiTest(void){
		u8 i=0;
	u16 j=0;
	u16 colorTemp=0;		
	DrawTestPage("AI8051:");
	
	LCD_ShowString(lcddev.width-24,0,16,"RST",1);//显示清屏区域
	
	LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,BLACK); 
	POINT_COLOR=BLACK;
	
	
	LCD_Fill(0, 18+96 + 1, 319, 18+96 + 3, BLACK); // 再屏幕中间画一条线

	
	
	
	while(1)
	{	 	 
		if(Convert_Pos())			//触摸屏被按下
		{	
		 	if(tp_pixlcd.x<lcddev.width&&tp_pixlcd.y<lcddev.height)
			{	
				if(tp_pixlcd.x>(lcddev.width-24)&&tp_pixlcd.y<16)
				{
					DrawTestPage("AI8051:");//清除
					LCD_ShowString(lcddev.width-24,0,16,"RST",1);//显示清屏区域
					POINT_COLOR=colorTemp;
					LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,POINT_COLOR); 
					LCD_Fill(0, 96 + 1, 319, 96 + 3, BLACK); // 再屏幕中间画一条线
					POINT_COLOR=BLACK;
					POINT_COLOR=colorTemp;
					clean_canvas();
				}
				else if((tp_pixlcd.x>(lcddev.width-60)&&tp_pixlcd.x<(lcddev.width-50+20))&&tp_pixlcd.y<20)
				{
					LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,ColorTab[j%5]); 
						POINT_COLOR=ColorTab[(j++)%5];
						colorTemp=POINT_COLOR;
						delay_ms(10);
					
				}else if(tp_pixlcd.x>0&&tp_pixlcd.x<lcddev.width&&tp_pixlcd.y>20&&tp_pixlcd.y<lcddev.height){
					if(tp_pixlcd.y<18+96){
						TP_Draw_Big_Point(tp_pixlcd.x,tp_pixlcd.y,POINT_COLOR); 
						
						canvas[(tp_pixlcd.x / 4) + ((tp_pixlcd.y-18) / 4) * CANVAS_WIDTH] = 255;
						
					}else if(tp_pixlcd.y>18+96){
						countResult();
						
					}
				}
				 
					   			   
			}
		}else delay_ms(5);	//没有按键按下的时候 	    				
	}   
}