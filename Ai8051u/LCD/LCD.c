#include "sys.h"
#include "lcd.h"

//LCD�Ļ�����ɫ�ͱ���ɫ       
u16 POINT_COLOR=0x0000;    //������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

void LCM_Config(void)
{
	LCMIFCFG = 0x00;	//bit7 1:Enable Interrupt, bit1 0:8bit mode; bit0 0:8080,1:6800
	LCMIFCFG2 = 0x25;	//RS:P45,E:P37,RW:P36; Setup Time=1,HOLD Time=1
	LCMIFSTA = 0x00;
	EA = 1;
}

void LCM_Interrupt(void) interrupt 13
{
	LCMIFSTA = 0x00;
	LCD_CS=1;
}


void LCD_WR_REG(u16 Reg)     
{
	LCMIFDATL = Reg;
	LCD_CS=0;
	LCMIFCR = 0x84;		//Enable interface, write command out
    while(!LCMIFSTA);
    LCMIFSTA = 0x00;
    LCD_CS=1;
} 


void LCD_WR_DATA(u16 Data)
{
	LCMIFDATL = Data;
	LCD_CS=0;
	LCMIFCR = 0x85;		//Enable interface, write data out
    while(!LCMIFSTA);
    LCMIFSTA = 0x00;
    LCD_CS=1;
}


u16 LCD_RD_DATA(void)
{
    u16 d;
    //LCM Read
    LCD_CS = 0;
    LCMIFCR = 0x87;		//Enable interface, Read data
    while(!LCMIFSTA);
    LCMIFSTA = 0x00;
    LCD_CS=1;
	d = LCMIFDATL;

    return (d);
}

  
void LCD_WR_DATA_16Bit(u16 Data)
{
	LCD_WR_DATA((u8)(Data>>8));
	LCD_WR_DATA((u8)Data);
}

u16 Color_To_565(u8 r, u8 g, u8 b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

 
u16 Lcd_RD_DATA_16Bit(void)
{
    u16 r,g,b;

    //dummy data
    r = LCD_RD_DATA();
    //dummy data
    r = LCD_RD_DATA();
    //8bit:red data
    //16bit:red and green data
    r = LCD_RD_DATA();
    //8bit:green data
    //16bit:blue data
    g = LCD_RD_DATA();

    b = LCD_RD_DATA();

    return Color_To_565((u8)r, (u8)g, (u8)b);
}


void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}


void LCD_ReadReg(u16 LCD_Reg,u8 *Rval,int n)
{
    LCD_WR_REG((u8)LCD_Reg);
    while(n--)
    {		
        *(Rval++) = LCD_RD_DATA();
    }
}

 
void LCD_WriteRAM_Prepare(void)
{
     LCD_WR_REG(lcddev.wramcmd);      
}

 
void LCD_ReadRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.rramcmd);
}


void LCD_Clear(u16 Color)
{
    u16 i,j;
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);    
    for(i=0;i<lcddev.width;i++)
    {
        for (j=0;j<lcddev.height;j++)
        {
            LCD_WR_DATA_16Bit(Color);
        }
    }
}

 
void LCD_DrawPoint(u16 x,u16 y)
{
    LCD_SetWindows(x,y,x,y);//���ù��λ�� 
    LCD_WR_DATA_16Bit(POINT_COLOR);         
}      

  
u16 LCD_ReadPoint(u16 x,u16 y)
{
    u16 color;
    if(x>=lcddev.width||y>=lcddev.height)
    {
        return 0;	//�����˷�Χ,ֱ�ӷ���	
    }
    LCD_SetCursor(x,y);//���ù��λ�� 
    LCD_ReadRAM_Prepare();
    color = Lcd_RD_DATA_16Bit();
    return color;
}


void LCD_Set_GPIO(void)
{
    //P2�����ó�׼˫���
    P2M0=0x00;
    P2M1=0x00;

    //P0.4�����óɸ�������,P0.5�������
    P0M0=0x20;
    P0M1=0x10;

    //P1.4�����óɸ�������
    P1M0=0x00;
    P1M1=0x10;

//    //P6�����óɸ�������(8bit)
//    P6M0=0x00;
//    P6M1=0xff;

    //P3.3�����óɸ�������
    //P3.7,P3.6,P3.4,P3.2�����ó��������
    P3M0=0xd4;
    P3M1=0x08;

    //P4.7,P4.5�����ó��������
    P4M0=0xa0;
    P4M1=0x00;

    //P5.3�����ó��������
    P5M0=0x08;
    P5M1=0x00;
}

  
void LCDReset(void)
{
    delay_ms(50);    
    LCD_RESET=0;
    delay_ms(50);
    LCD_RESET=1;
    delay_ms(50);
}

        
void LCD_Init(void)
{
    LCD_Set_GPIO();
    LCM_Config();
    LCDReset(); //��ʼ��֮ǰ��λ
//*************2.4inch ILI9341��ʼ��**********//
    LCD_WR_REG(0xCF);  
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0xD9); //0xC1 
    LCD_WR_DATA(0X30); 
    LCD_WR_REG(0xED);  
    LCD_WR_DATA(0x64); 
    LCD_WR_DATA(0x03); 
    LCD_WR_DATA(0X12); 
    LCD_WR_DATA(0X81); 
    LCD_WR_REG(0xE8);  
    LCD_WR_DATA(0x85); 
    LCD_WR_DATA(0x10); 
    LCD_WR_DATA(0x7A); 
    LCD_WR_REG(0xCB);  
    LCD_WR_DATA(0x39); 
    LCD_WR_DATA(0x2C); 
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0x34); 
    LCD_WR_DATA(0x02); 
    LCD_WR_REG(0xF7);  
    LCD_WR_DATA(0x20); 
    LCD_WR_REG(0xEA);  
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0x00); 
    LCD_WR_REG(0xC0);    //Power control 
    LCD_WR_DATA(0x1B);   //VRH[5:0] 
    LCD_WR_REG(0xC1);    //Power control 
    LCD_WR_DATA(0x12);   //SAP[2:0];BT[3:0] 0x01
    LCD_WR_REG(0xC5);    //VCM control 
    LCD_WR_DATA(0x08);      //30
    LCD_WR_DATA(0x26);      //30
    LCD_WR_REG(0xC7);    //VCM control2 
    LCD_WR_DATA(0XB7); 
    LCD_WR_REG(0x36);    // Memory Access Control 
    LCD_WR_DATA(0x08);
    LCD_WR_REG(0x3A);   
    LCD_WR_DATA(0x55); 
    LCD_WR_REG(0xB1);   
    LCD_WR_DATA(0x00);   
    LCD_WR_DATA(0x1A); 
    LCD_WR_REG(0xB6);    // Display Function Control 
    LCD_WR_DATA(0x0A); 
    LCD_WR_DATA(0xA2); 
    LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
    LCD_WR_DATA(0x00); 
    LCD_WR_REG(0x26);    //Gamma curve selected 
    LCD_WR_DATA(0x01); 
    LCD_WR_REG(0xE0);    //Set Gamma 
    LCD_WR_DATA(0x0F); 
    LCD_WR_DATA(0x1D); 
    LCD_WR_DATA(0x1A); 
    LCD_WR_DATA(0x0A); 
    LCD_WR_DATA(0x0D); 
    LCD_WR_DATA(0x07); 
    LCD_WR_DATA(0x49); 
    LCD_WR_DATA(0X66); 
    LCD_WR_DATA(0x3B); 
    LCD_WR_DATA(0x07); 
    LCD_WR_DATA(0x11); 
    LCD_WR_DATA(0x01); 
    LCD_WR_DATA(0x09); 
    LCD_WR_DATA(0x05); 
    LCD_WR_DATA(0x04);          
    LCD_WR_REG(0XE1);    //Set Gamma 
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0x18); 
    LCD_WR_DATA(0x1D); 
    LCD_WR_DATA(0x02); 
    LCD_WR_DATA(0x0F); 
    LCD_WR_DATA(0x04); 
    LCD_WR_DATA(0x36); 
    LCD_WR_DATA(0x13); 
    LCD_WR_DATA(0x4C); 
    LCD_WR_DATA(0x07); 
    LCD_WR_DATA(0x13); 
    LCD_WR_DATA(0x0F); 
    LCD_WR_DATA(0x2E); 
    LCD_WR_DATA(0x2F); 
    LCD_WR_DATA(0x05); 
    LCD_WR_REG(0x2B); 
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x3f);
    LCD_WR_REG(0x2A); 
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xef);     
    LCD_WR_REG(0x11); //Exit Sleep
    delay_ms(120);
    LCD_WR_REG(0x29); //display on    

    //����LCD���Բ���
    LCD_direction(USE_HORIZONTAL);//����LCD��ʾ���� 
//    LCD_BL=1;//��������     
}


void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{
    LCD_WR_REG(lcddev.setxcmd);    
    LCD_WR_DATA(xStar>>8);
    LCD_WR_DATA(0x00FF&xStar);        
    LCD_WR_DATA(xEnd>>8);
    LCD_WR_DATA(0x00FF&xEnd);

    LCD_WR_REG(lcddev.setycmd);    
    LCD_WR_DATA(yStar>>8);
    LCD_WR_DATA(0x00FF&yStar);        
    LCD_WR_DATA(yEnd>>8);
    LCD_WR_DATA(0x00FF&yEnd);    

    LCD_WriteRAM_Prepare();    //��ʼд��GRAM
}


void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);    
}


void LCD_direction(u8 direction)
{
    lcddev.setxcmd=0x2A;
    lcddev.setycmd=0x2B;
    lcddev.wramcmd=0x2C;
    lcddev.rramcmd=0x2E;
    switch(direction){
        case 0:
            lcddev.width=LCD_W;
            lcddev.height=LCD_H;
            LCD_WriteReg(0x36,(1<<3));
        break;
        case 1:
            lcddev.width=LCD_H;
            lcddev.height=LCD_W;
            LCD_WriteReg(0x36,(1<<3)|(1<<5)|(1<<6));
        break;
        case 2:
            lcddev.width=LCD_W;
            lcddev.height=LCD_H;    
            LCD_WriteReg(0x36,(1<<3)|(1<<4)|(1<<6)|(1<<7));
        break;
        case 3:
            lcddev.width=LCD_H;
            lcddev.height=LCD_W;
            LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5)|(1<<4));
        break;
        default:break;
    }
}


u16 LCD_Read_ID(void)
{
    u8 val[5] = {0};
    LCD_ReadReg(0xD3,val,5);
    return (val[3]<<8)|val[4];
}
