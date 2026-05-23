//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: SC_it.c
//	模块功能	: 中断服务程序
//  说明        :
//*************************************************************
/********************Includes************************************************************************/
#include "SC_it.h"

void INT0Interrupt()		interrupt 0
{
    TCON &= 0XFD;//清中断标志位
    /*INT0_it write here*/
}

void Timer0Interrupt()		interrupt 1
{
    /*TIM0_it write here*/
}

void INT1Interrupt()		interrupt 2
{
    TCON &= 0XF7;//清中断标志位
    /*INT1_it write here*/
}

void Timer1Interrupt()		interrupt 3
{
    /*TIM1_it write here*/
}
/*
#if defined (SC92F854x) || defined (SC92F754x) ||defined  (SC92F844xB) || defined (SC92F744xB)||defined  (SC92F84Ax_2) || defined (SC92F74Ax_2)|| defined (SC92F846xB) \
|| defined (SC92F746xB) || defined (SC92F836xB) || defined (SC92F736xB) || defined (SC92F8003)||defined  (SC92F84Ax) || defined (SC92F74Ax) || defined  (SC92F83Ax) \
|| defined (SC92F73Ax) || defined (SC92F7003) || defined (SC92F725x) || defined (SC92F730x) || defined (SC92F732x) || defined (SC92F735x) || defined (SC92F740x)\
|| defined (SC92FWxx) || defined (SC93F743x) || defined (SC93F833x) || defined (SC93F843x) || defined (SC92F848x) || defined (SC92F748x) || defined (SC92F859x) \
|| defined (SC92F759x) 
#endif
*/
void UART0Interrupt()		interrupt 4		
{

    /*UART0_it write here*/		
}

#if defined (SC92F742x) || defined (SC92F7490)
void SSI0Interrupt()		interrupt 4		
{
    /*SSI0_it write here*/		
}

void SSI1Interrupt()		interrupt 7		
{

    /*SSI1_it write here*/		
}

#endif

void Timer2Interrupt()		interrupt 5
{
	
    /*TIM2_it write here*/
}

void ADCInterrupt()			interrupt 6
{

    /*ADC_it write here*/
}


void PWMInterrupt()			interrupt 8
{
	
    /*PWM_it write here*/
}

#if !defined (TK_USE_BTM)
void BTMInterrupt()			interrupt 9
{
    /*BTM_it write here*/
}
#endif

void INT2Interrupt()		interrupt 10
{
    /*INT2_it write here*/
}



#if 0
#if defined (SC92F854x) || defined (SC92F754x) ||defined  (SC92F844xB) || defined (SC92F744xB)||defined  (SC92F84Ax_2) || defined (SC92F74Ax_2)|| defined (SC92F846xB) \
|| defined (SC92F746xB) || defined (SC92F836xB) || defined (SC92F736xB) || defined (SC92F8003)||defined  (SC92F84Ax) || defined (SC92F74Ax) || defined  (SC92F83Ax) \
|| defined (SC92F73Ax) || defined (SC92F7003) || defined (SC92F740x) || defined (SC92FWxx) || defined (SC93F743x) || defined (SC93F833x) || defined (SC93F843x) \
|| defined (SC92F848x) || defined (SC92F748x) || defined (SC92F859x) || defined (SC92F759x) 
#endif

void SSIInterrupt()			interrupt 7		
{
    /*SSI_it write here*/		
}

#if defined (SC92F854x) || defined (SC92F754x) ||defined  (SC92F844xB) || defined (SC92F744xB)||defined  (SC92F84Ax_2) || defined (SC92F74Ax_2) \
|| defined (SC92F859x) || defined (SC92F759x) 
void ACMPInterrupt()		interrupt 12
{
/*<UserCodeStart>*/    
/*<UserCodeEnd>*/
    /*ACMP_it write here*/		
}
#endif
#endif
