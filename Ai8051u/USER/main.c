#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include "Ai.h"
//������
void main(void)
{ 
    WTST = 0;  //���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
    EAXFR = 1; //��չ�Ĵ���(XFR)����ʹ��
    CKCON = 0; //��߷���XRAM�ٶ�

    //Һ������ʼ��
    LCD_Init();
		Ai_init();
	
    //ѭ�����и������    
    while(1)
    {
        aiTest();
    }
}
