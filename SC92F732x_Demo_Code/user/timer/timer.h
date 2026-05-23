
/**********************************************************/
/* timer.h file
/* 
/* Copyright (c) 2013 davidd
/*
/***********************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__
/* Includes ----------------------------------------------------------------*/

/* public defines -----------------------------------------------------------*/

#define WDG_ReloadCounter	WDTCON|=SET_BIT4	//IWDG_ReloadCounter()						
/* Exported variables ------------------------------------------------------- */
//extern volatile unsigned short TimCnt20uS;			// timer2
//extern volatile unsigned short TimCnt100uS;			// timer0
extern bit_field_t TimFlg;
#define Tim1ms_flg TimFlg.bits.b0
#define Tim10ms_flg TimFlg.bits.b1


#define TNORMAL 	0
#define MAX100US	1

#define TUART0		1
#define TTCPHND		2
#define TLED_INFO	3
#define TINP		4

#define MAX1MS		6


//#define TKEYINPUT	1
//#define TPWM		2
//#define TINFO		3
//#define TLOAD		1
//#define TBEEP		3
//#define TWATER_DET	1
#define TKEYWAIT	1
#define TWATR_HAND	2

#define TCKOFF		3
#define TACK_TMO	4
#define TPAIR_SEQ	5
#define TPAIR_RSP	6
#define MAX10MS		7

#define MAX1S		1
//extern volatile unsigned short TimOut100uS[MAX100US];
extern volatile unsigned short TimOut1mS[MAX1MS]; 
extern volatile unsigned short TimOut10mS[MAX10MS]; 
//extern volatile unsigned short TimOut1S[MAX1S]; 
// used in the TIM4 ovf interrupt end

#define TIMOUT_UART_1MS			5

#define TIMOUT_KEYIN_1MS			20

#define TIMOUT_KEYJITTER_10MS		6
#define TIMOUT_KEYCP_10MS			200
#define TIMOUT_KEYSCP_10MS			400
#define TIMOUT_KEYCPH_10MS			50

#define TIMOUT_PWM_10MS				10

#define TIMOUT_INFO_10MS			50	

/* Exported functions ------------------------------------------------------- */

#define OP_HRCR		0x83
#define OP_CTM0		0xC1
#define OP_CTM1		0xC2

void write_option(uint8_t addr,  uint8_t val);
void Set_Option0_Bit(uint8_t addr,  uint8_t uBits);
void Reset_Option0_Bit(uint8_t addr,  uint8_t uBits);

#define Enable_Wdt	Set_Option0_Bit(OP_CTM0, BIT7)
#define Disable_Wdt	Reset_Option0_Bit(OP_CTM0, BIT7)

#define Enable_LVR	Reset_Option0_Bit(OP_CTM0, BIT2)
#define Disable_LVR	Set_Option0_Bit(OP_CTM0, BIT2)

void halt(void);
void Idle(void);
void WaitWakeUp(void);
extern uint16_t idata w_SleepCnt;

void Init_wdgt(void);
void IWDG_ReloadCounter(void);

void Delay0_1us(unsigned short nCount);
void Delay100us(unsigned short nCount);
void Delay0_1ms(unsigned char nCount);
void Delay_1ms(unsigned short nCount);
void TimFlg_Hand(void);


extern bit_field_t data B_Sys_Flag;
#define f_Sens_ok		B_Sys_Flag.bits.b0		// 默认值0: 	水平开关已到位标记;
// #define f_mKeyDown		B_Sys_Flag.bits.b1		// 默认值0:		// 0: 锁定标记; 1: 污水已经满1分钟后, 喷水和吸尘已经停止
// #define f_mKeyLong		B_Sys_Flag.bits.b2		// 默认值0:	


extern bit_field_t data B_Sys_Flag2;
// #define f_req_spring	B_Sys_Flag2.bits.b0		// 默认值0: 请求喷水标记;        	1: 表示有喷水需求;
// #define f_Water_Spring	B_Sys_Flag2.bits.b1		// 默认值0:		//  0-不在喷水; 1-在喷水
// #define f_LedOn			B_Sys_Flag2.bits.b2		// 默认值0:		// 0: 表示Led灭, 1表示Led 亮


// #define b_IsFirstWater	B_Sys_Flag.bits.b7		// 默认值1			//	1: 表示刚开机,第一次喷水
#if 0
extern volatile bit_field_t B_Cap_Flag;
#define b_Cap_a		B_Cap_Flag.bits.b0			// 默认值0	
#define b_Cap_c		B_Cap_Flag.bits.b1			// 默认值0	 	//	3Hall中判断时用

// davidd 20250605, 改完后拉时喷水, 所以b_dir_Front表示后退
#define b_dir_Front		B_Cap_Flag.bits.b2		// 默认值0		//  1:表示当下在前进,        0:当下在后退;
#define b_dir_Frontbk	B_Cap_Flag.bits.b3		// 默认值0		//  1:表示当下在前进,        0:当下在后退;
#endif
#endif





