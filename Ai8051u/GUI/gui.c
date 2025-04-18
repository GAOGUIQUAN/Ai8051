#include "lcd.h"
#include "font.h" 
#include "sys.h"
#include "gui.h"
#include "string.h"


void GUI_DrawPoint(u16 x,u16 y,u16 color)
{
    LCD_SetCursor(x,y);//设置光标位置 
    LCD_WR_DATA_16Bit(color); 
}

void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{      
    u16 i,j;            
    u16 width=ex-sx+1;         //得到填充的宽度
    u16 height=ey-sy+1;        //高度
    LCD_SetWindows(sx,sy,ex,ey);//设置显示窗口
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        LCD_WR_DATA_16Bit(color);    //写入数据      
    }
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口设置为全屏
}


void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 t; 
    int xerr=0,yerr=0,delta_x,delta_y,distance; 
    int incx,incy,uRow,uCol; 

    delta_x=x2-x1; //计算坐标增量 
    delta_y=y2-y1; 
    uRow=x1; 
    uCol=y1; 
    if(delta_x>0)incx=1; //设置单步方向 
    else if(delta_x==0)incx=0;//垂直线 
    else {incx=-1;delta_x=-delta_x;} 
    if(delta_y>0)incy=1; 
    else if(delta_y==0)incy=0;//水平线 
    else{incy=-1;delta_y=-delta_y;} 
    if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
    else distance=delta_y; 
    for(t=0;t<=distance+1;t++ )//画线输出 
    {  
        LCD_DrawPoint(uRow,uCol);//画点 
        xerr+=delta_x ; 
        yerr+=delta_y ; 
        if(xerr>distance) 
        { 
            xerr-=distance; 
            uRow+=incx; 
        } 
        if(yerr>distance) 
        { 
            yerr-=distance; 
            uCol+=incy; 
        } 
    }  
} 

void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
{  
    u8 temp;
    u8 pos,t;
    u16 colortemp=POINT_COLOR;      

    num=num-' ';//得到偏移后的值
    LCD_SetWindows(x,y,x+size/2-1,y+size-1);//设置单个文字显示窗口
    if(!mode) //非叠加方式
    {
        for(pos=0;pos<size;pos++)
        {
            if(size==12)temp=asc2_1206[num].dat[pos];//调用1206字体
            else temp=asc2_1608[num].dat[pos];         //调用1608字体
            for(t=0;t<size/2;t++)
            {                 
                if(temp&0x01)LCD_WR_DATA_16Bit(fc); 
                else LCD_WR_DATA_16Bit(bc); 
                temp>>=1; 
                
            }
        }    
    }else//叠加方式
    {
        for(pos=0;pos<size;pos++)
        {
            if(size==12)temp=asc2_1206[num].dat[pos];//调用1206字体
            else temp=asc2_1608[num].dat[pos];         //调用1608字体
            for(t=0;t<size/2;t++)
            {   
                POINT_COLOR=fc;              
                if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//画一个点    
                temp>>=1; 
            }
        }
    }
    POINT_COLOR=colortemp;    
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏                      
}

u32 mypow(u8 m,u8 n)
{
    u32 result=1;     
    while(n--)result*=m;    
    return result;
}

void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{             
    u8 t,temp;
    u8 enshow=0;                           
    for(t=0;t<len;t++)
    {
        temp=(num/mypow(10,(u8)(len-t-1)))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,' ',size,0);
                continue;
            }else enshow=1; 
              
        }
         LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,(u8)(temp+'0'),size,0); 
    }
} 

void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
{         
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {   
        if(x>(lcddev.width-1)||y>(lcddev.height-1)) 
        return;     
        LCD_ShowChar(x,y,POINT_COLOR,BACK_COLOR,*p,size,mode);
        x+=size/2;
        p++;
    }  
} 

void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
{
    u8 i,j;
    u16 k;
    u16 HZnum;
    u16 x0=x;
    HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);    //自动统计汉字数目
    
    for (k=0;k<HZnum;k++) 
    {
        if ((tfont16[k].txt[0]==*(s))&&(tfont16[k].txt[1]==*(s+1)))
        {
            LCD_SetWindows(x,y,x+16-1,y+16-1);
            for(i=0;i<16*2;i++)
            {
                for(j=0;j<8;j++)
                {    
                    if(!mode) //非叠加方式
                    {
                        if(tfont16[k].dat[i]&(0x80>>j))    LCD_WR_DATA_16Bit(fc);
                        else LCD_WR_DATA_16Bit(bc);
                    }
                    else
                    {
                        POINT_COLOR=fc;
                        if(tfont16[k].dat[i]&(0x80>>j))    LCD_DrawPoint(x,y);//画一个点
                        x++;
                        if((x-x0)==16)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }                      
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
} 

void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
{
    u8 i,j;
    u16 k;
    u16 HZnum;
    u16 x0=x;
    HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);    //自动统计汉字数目

    for (k=0;k<HZnum;k++) 
    {
        if ((tfont24[k].txt[0]==*(s))&&(tfont24[k].txt[1]==*(s+1)))
        {
            LCD_SetWindows(x,y,x+24-1,y+24-1);
            for(i=0;i<24*3;i++)
            {
                    for(j=0;j<8;j++)
                    {
                        if(!mode) //非叠加方式
                        {
                            if(tfont24[k].dat[i]&(0x80>>j))    LCD_WR_DATA_16Bit(fc);
                            else LCD_WR_DATA_16Bit(bc);
                        }
                    else
                    {
                        POINT_COLOR=fc;
                        if(tfont24[k].dat[i]&(0x80>>j))    LCD_DrawPoint(x,y);//画一个点
                        x++;
                        if((x-x0)==24)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }                      
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
}


void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
{
    u8 i,j;
    u16 k;
    u16 HZnum;
    u16 x0=x;
    HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);    //自动统计汉字数目
    for (k=0;k<HZnum;k++) 
    {
        if ((tfont32[k].txt[0]==*(s))&&(tfont32[k].txt[1]==*(s+1)))
        {
            LCD_SetWindows(x,y,x+32-1,y+32-1);
            for(i=0;i<32*4;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode) //非叠加方式
                    {
                        if(tfont32[k].dat[i]&(0x80>>j))    LCD_WR_DATA_16Bit(fc);
                        else LCD_WR_DATA_16Bit(bc);
                    }
                    else
                    {
                        POINT_COLOR=fc;
                        if(tfont32[k].dat[i]&(0x80>>j))    LCD_DrawPoint(x,y);//画一个点
                        x++;
                        if((x-x0)==32)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }                      
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
} 

void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{                    
    u16 x0=x;                                    
    u8 bHz=0;     //字符或者中文 
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
            if(x>(lcddev.width-size/2)||y>(lcddev.height-size)) 
            return; 
            if(*str>0x80)bHz=1;//中文 
            else              //字符
            {          
                if(*str==0x0D)//换行符号
                {         
                    y+=size;
                    x=x0;
                    str++; 
                }  
                else
                {
                    if(size>16)//字库中没有集成12X24 16X32的英文字体,用8X16代替
                    {  
                    LCD_ShowChar(x,y,fc,bc,*str,16,mode);
                    x+=8; //字符,为全字的一半 
                    }
                    else
                    {
                    LCD_ShowChar(x,y,fc,bc,*str,size,mode);
                    x+=size/2; //字符,为全字的一半 
                    }
                } 
                str++; 
            }
        }
        else//中文 
        {   
            if(x>(lcddev.width-size)||y>(lcddev.height-size)) 
            return;  
            bHz=0;//有汉字库    
            if(size==32)
            GUI_DrawFont32(x,y,fc,bc,str,mode);         
            else if(size==24)
            GUI_DrawFont24(x,y,fc,bc,str,mode);    
            else
            GUI_DrawFont16(x,y,fc,bc,str,mode);
                
            str+=2; 
            x+=size;//下一个汉字偏移        
        }                         
    }   
}

void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
{
    GUI_DrawPoint(xc + x, yc + y, c);

    GUI_DrawPoint(xc - x, yc + y, c);

    GUI_DrawPoint(xc + x, yc - y, c);

    GUI_DrawPoint(xc - x, yc - y, c);

    GUI_DrawPoint(xc + y, yc + x, c);

    GUI_DrawPoint(xc - y, yc + x, c);

    GUI_DrawPoint(xc + y, yc - x, c);

    GUI_DrawPoint(xc - y, yc - x, c);
}

void gui_circle(int xc, int yc,u16 c,int r, int fill)
{
    int x = 0, y = r, yi, d;

    d = 3 - 2 * r;

    if (fill) 
    {
        // 如果填充（画实心圆）
        while (x <= y) {
            for (yi = x; yi <= y; yi++)
                _draw_circle_8(xc, yc, x, yi, c);

            if (d < 0) {
                d = d + 4 * x + 6;
            } else {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    } else 
    {
        // 如果不填充（画空心圆）
        while (x <= y) {
            _draw_circle_8(xc, yc, x, y, c);
            if (d < 0) {
                d = d + 4 * x + 6;
            } else {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
}



void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{
    u16 len=strlen((const char *)str);
    u16 x1=(lcddev.width-len*8)/2;
    Show_Str(x+x1,y,fc,bc,str,size,mode);
} 
 


