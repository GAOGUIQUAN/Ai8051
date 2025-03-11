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
u16 ColorTab[5]={RED,GREEN,BLUE,YELLOW,BRED};//������ɫ����
u16 ColornTab[8]={RED,MAGENTA,GREEN,DARKBLUE,BLUE,BLACK,LIGHTGREEN};
//=====================end of variable======================//


void DrawTestPage(u8 *str)
{
    //���ƹ̶���up
    LCD_Clear(WHITE);
    LCD_Fill(0,0,lcddev.width,20,BLUE);
    //���ƹ̶���down
    LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
    POINT_COLOR=WHITE;
    Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//������ʾ
    Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"AI calculator",16,1);//������ʾ
    //���Ʋ�������
    //LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
}

void paddingString(uint8_t *buffer, char c, uint8_t new_length)
{
	uint8_t current_length;
	uint8_t i;

	current_length = (uint8_t)strlen((char *)buffer); // ��ȡ��ǰ�ַ�������

	// �����ǰ�����Ѿ����ڻ�����³��ȣ����������
	if (current_length >= new_length)
	{
		return;
	}

	// �ӵ�ǰ�ַ���ĩβ��ʼ��䣬ֱ���ﵽ�³���
	for (i = current_length; i < new_length; ++i)
	{
		buffer[i] = c;
	}

	// ����ַ�����ֹ��
	buffer[new_length] = '\0';
}


uint8_t xdata expression[16] = {0};
uint8_t xdata expression_n = 0;
static uint8_t xdata strBuffer[50] = {0};

void countResult(void){
	{
				uint32_t x, y;
				uint8_t sum;
				int in_char = 0;		// ��ǵ�ǰ�Ƿ����ַ�����
				int char_count = 0;		// �ַ���������
				uint32_t start_col = 0; // �ַ���ʼ��
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
						// ��ǰ���з���ֵ���������ַ�������
						if (!in_char)
						{
							in_char = 1;   // ��ǽ����ַ�����
							start_col = x; // ��¼�ַ���ʼ��
							char_count++;  // �ַ���������
						}
					}
					else
					{
						// ��ǰ��ȫΪ�㣬���������ַ�������
						if (in_char)
						{
							uint32_t end_col;
							in_char = 0; // ����뿪�ַ�����
							end_col = x - 1;

							if (canvas_process_character(start_col, end_col))
							{
								expression[expression_n++] = Ai_run();
							}
						}
					}
				}

				// �������һ���ַ���ͼƬĩβ�����
				if (in_char)
				{
					// ���һ���ַ��Ľ�����ΪͼƬĩβ
					uint32_t end_col = CANVAS_WIDTH - 1;

					if (canvas_process_character(start_col, end_col))
					{
						expression[expression_n++] = Ai_run();
					}
					// printf("�ַ� %d ", char_count);
					// printf("��ʼ��: %d,", start_col);
					// printf("������: %d\n", end_col);
				}

				sprintf(strBuffer, "result: %.2f", expression_calc(expression, expression_n));
				// �����ַ�����ƴ�ӿո�ķ�������������ַ�������ȫ��ˢ����ɵ���˸��
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
	
	LCD_ShowString(lcddev.width-24,0,16,"RST",1);//��ʾ��������
	
	LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,BLACK); 
	POINT_COLOR=BLACK;
	
	
	LCD_Fill(0, 18+96 + 1, 319, 18+96 + 3, BLACK); // ����Ļ�м仭һ����

	
	
	
	while(1)
	{	 	 
		if(Convert_Pos())			//������������
		{	
		 	if(tp_pixlcd.x<lcddev.width&&tp_pixlcd.y<lcddev.height)
			{	
				if(tp_pixlcd.x>(lcddev.width-24)&&tp_pixlcd.y<16)
				{
					DrawTestPage("AI8051:");//���
					LCD_ShowString(lcddev.width-24,0,16,"RST",1);//��ʾ��������
					POINT_COLOR=colorTemp;
					LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,POINT_COLOR); 
					LCD_Fill(0, 96 + 1, 319, 96 + 3, BLACK); // ����Ļ�м仭һ����
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
		}else delay_ms(5);	//û�а������µ�ʱ�� 	    				
	}   
}