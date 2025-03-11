#ifndef __SYS_H
#define __SYS_H	
#include <intrins.h>
#include "AI8051U.h"

#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long

#define     MAIN_Fosc       40000000L   //定义主时钟

void delay_ms(int count);
void delay_us(int count);

	  		 
#endif  
	 
	 



//#ifndef COMMON_H
//#define COMMON_H

//#include "AI8051U.h"
//#include "stdio.h"
//#include "Type_def.h"
//#include "intrins.h"

//#define MAIN_Fosc        40000000UL

//#endif
