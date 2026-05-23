#ifndef _Function_Init_H_
#define _Function_Init_H_

#if 0
#include "SC92F732x_C.H"
#include "intrins.H"
#define uchar unsigned char
#define uint unsigned int

#define IapROM  0x00	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作
#define IapEPPROM  0x02	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作
#endif
extern void IO_Init(void);
extern void BTM_Test(void);
extern void EXTI_Test(void);
extern void Timer_Test(void);
extern void LCD_Test(void);
extern void PWM_Test(void);
extern void Uart0_Test(void);
extern void ADC_Test(void);
extern void IAP_Test(uint Address,uchar IAPArea);


void Timer_Init(void);
void Timer0_Init(void);		// 100us 定时
void Timer1_Init(void);		// 20us 定时，一般慎用
void Timer2_Init(void);

void PWM_Init(void);
void ADC_Init(uint Channel);

void Uart0_Init_T1(uint32_t u32Baudrate);    //选择Timer1作为波特率信号发生器
// void Uart0_Init_T2(uint32_t u32Baudrate);    //选择Timer2作为波特率信号发生器
void Uart0_Init_T2(void);    //选择Timer2作为波特率信号发生器

void EX_Init(void);

/*****************************************
说明：选择测试IC
*****************************************/
#define SC92F7352  0
#define SC92F7351  1
#define SC92F7350  2

#define SC92F7323  3
#define SC92F7322  4
#define SC92F7321  5
#define SC92F7320  6

#define IC_MODEL  SC92F7322

#endif
