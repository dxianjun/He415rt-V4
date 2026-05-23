/**********************************************************/
/* timer.c file
/* 
/* Copyright (c) 2013 davidd
/*
/***********************************************************/
/* Private define ------------------------------------------------------------*/
#define __TIMER__

/* -------------------------------Includes --------------------------*/
#include "global.h"

/* -------------------------------variables --------------------------*/

/***********************************************************************/
// timer
/*
timer0: 产生时基1ms
timer1: 产生时基125us, 用于433等的采样
timer2: 用于串口, 或者捕获
*/

// volatile unsigned short TimCnt100uS;	
//volatile unsigned short TimOut100uS[MAX100US]={0};

volatile unsigned short TimCnt1mS;	

bit_field_t TimFlg;

volatile unsigned short TimOut1mS[MAX1MS]={0};
volatile unsigned short TimOut10mS[MAX10MS]={0}; 
volatile unsigned short TimOut1S[MAX1S]={0}; 

//extern bit b_IsInitOk;

		
// volatile uint16_t ui_Tmr_10ms=0;		// 用于脉冲数的精确定位

#if 1
/************************************************************************************************************
*    TIMER 0 interrupt subroutine
************************************************************************************************************/
void Timer0_ISR (void) interrupt 1              //interrupt address is 0x000B
{
	#if 1
	// 如果是16bit模式1，要软件重写，如果是自动重载的8bit模式2，就不用重写了, 此时要用125us的
	uint16_t u_dly;
	
	u_dly = 65536 - TIMER0_RELOAD_VALUE_1ms;

	TL0 = LOBYTE(u_dly);    //溢出时间：时钟为Fsys，则125*（1/Fsys）=125us;
	TH0 = HIBYTE(u_dly);
	#endif
	
	// GPIO1 toggle when interrupt
	// Timer0 interrupt every 1ms
	TimCnt1mS++;
	Tim1ms_flg=1;
	
	if(TimCnt1mS % 10 ==0)
		{
		Tim10ms_flg=1;
		
		if (TimCnt1mS >= 50000)
			{
			TimCnt1mS = 0;
			}
		}
}
#endif
// 放主循环, 时间会略有不准;
void TimFlg_Hand(void)
{
	unsigned char i=0;
	
	if (Tim1ms_flg)
		{
		Tim1ms_flg = 0;

		for (i=0;i<MAX1MS;i++)
			{
			TimOut1mS[i]++;
			}
		}

	if (Tim10ms_flg)
		{
		Tim10ms_flg = 0;

		for (i=0;i<MAX10MS;i++)
			{
			TimOut10mS[i]++;
			}
		}
}

void Delay0_1us(unsigned short  nCount) //	1M 1uS
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
    	nCount--;
    }
}

#if 0
void Delay0_1ms(unsigned char nCount)
{
	TimOut100uS[TNORMAL] = 0;
	while (TimOut100uS[TNORMAL] < nCount+1);
}
#endif

void Delay_1ms(unsigned short nCount)
{
	unsigned short uCountH=0;
	uCountH = nCount/100;
	while (uCountH--)
		{
		WDG_ReloadCounter;
		TimOut1mS[TNORMAL] = 0;
		while (TimOut1mS[TNORMAL] < 100)
			{
			TimFlg_Hand();
			}
		}
	WDG_ReloadCounter;
	uCountH = nCount%100;
	TimOut1mS[TNORMAL] = 0;
	while (TimOut1mS[TNORMAL] < uCountH)
		{
		TimFlg_Hand();
		}
	
	if (nCount%100 >= 50)
		WDG_ReloadCounter;
}

#if 0
void Init_wdgt(void)
{
	WDTCON=0x00;  		//Setting WDT prescale 			// 000-- 500ms,  001--250ms, ... 111--3.94ms   :500*2^(-n),
	set_WDCLR;											//Clear WDT timer
}

void IWDG_ReloadCounter(void)
{
	set_WDCLR;
}
#endif

#if 0
#define CKSWTF	SET_BIT0
#define HIRCST	SET_BIT5


void SwitchFsys(bit bHirc)	
{
 bHirc=!bHirc;
}
#endif

void write_option(uint8_t addr,  uint8_t val)
{
	if ((addr!=OP_HRCR) && (addr!=OP_CTM0) && (addr!=OP_CTM1)) 
		return;
	
	OPINX = addr;
	OPREG = val;
}
#if 0
void Idle(void)
{
	// wdt off, enx off, sclk 12M, Rst, lvr off, lvr 3.7 ---0x16
	// wdt on, enxoff, sclk 12M, rst, Lvr on, lvr 3.7V -- 0x92
	
	write_option(OP_CTM0, 0x16);	

	_nop_();
	_nop_();
	_nop_();
	_nop_();
	
	set_IDL;			// 进入待机-IDLE模式；

	_nop_(); 			//至少需要 8 个_nop_()
	_nop_();
	_nop_();
	_nop_();
	
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}
#endif

// wdt off, enx off, sclk 12M, Rst, lvr off, lvr 3.7 ---0x16  lvr2.3v = 0x14
// wdt on, enxoff, sclk 12M, rst, Lvr on, lvr 3.7V -- 0x92 lvr2.3v = 0x90

#define LVR_2_3V_OFF	0x14
#define LVR_2_3V_ON		0x90

#define LVR_3_7V_OFF	0x16
#define LVR_3_7V_ON		0x92

#define VDD_33V		1		// 0: 5.0V, 1:3.3V

void halt(void)
{

	#if VDD_33V
	write_option(OP_CTM0, LVR_2_3V_OFF);	
	#else
	write_option(OP_CTM0, LVR_3_7V_OFF);
	#endif
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	
	set_PD;				// 进入待机-掉电模式； //PCON 的 bit1 STOP 位写 1，配置 MCU 进入 STOP 模式

	_nop_(); 			//至少需要 8 个_nop_()
	_nop_();
	_nop_();
	_nop_();
	
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}


// 硬件有问题, 没办法做唤醒, 所以不做MCU待机, 只是把PWM和Led都关掉
// 待机
volatile bit bTrigWakeUp=FALSE;
void WaitWakeUp(void)
{
//	printf("halt\r\n");
	
	set_EINT0;
	while (1)
		{
		bTrigWakeUp = FALSE;
		WDG_ReloadCounter;
		halt(); 				//  进入断电模式
		
		WDG_ReloadCounter;
		//  假如发生唤醒而且检查 OK,  就返回正常工作
		if (bTrigWakeUp)
			break;

		//  否则，停留在断电模式。
		}
	
	// 从待机中唤醒
	
	#if VDD_33V
	write_option(OP_CTM0, LVR_2_3V_ON);	
	#else
	write_option(OP_CTM0, LVR_3_7V_ON);
	#endif
	// 唤醒后, 关闭中断
	clr_EINT0;
	//clr_EINT1;
	// clr_EINT2;
	
	bTrigWakeUp = FALSE;
	TimOut1mS[TCKOFF] = 0;

	// 重新wakeup后, 会将Low标记清除, 所以这里要设置low, davidd 20250225
	// davidd 20250918, 重新唤醒后，P02的锁定不能放开；
	// SET_INFO_LOW;
	printf("wake up\r\n");
}



