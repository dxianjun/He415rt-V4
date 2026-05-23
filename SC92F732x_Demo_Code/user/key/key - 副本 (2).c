/**********************************************************/
/* key.c file   for SC92F7322
/* 
/* Copyright (c) 2013 davidd
/* 
/*
/***********************************************************/
/* Private define ------------------------------------------------------------*/
#define __SC92_KEY__


/* -------------------------------Includes --------------------------*/
#include "global.h"

/* -------------------------------variables --------------------------*/
/*
#define IN_LEFT_ADJ			P24		// IPU 左转任意角度
#define IN_LEFT_180			P21		// IPU  左转180度
#define IN_RIGHT_ADJ		P20		// IPU  左转任意角度
#define IN_RIGHT_180		P05		// IPU  左转180度

#define IN_K4_PIN			P04		// IPU  大水量
#define IN_K3_PIN			P03		// IPU  小水量
#define IN_K2_PIN			P01		// IPU  停止
#define IN_K1_PIN			P00		// IPU  开关按键
*/

/***********************************************************************/

/* -------------------------------Function --------------------------*/
void InitKeyGpio(void)
{
	// 输入上拉
	P00_Quasi_Mode;
	P01_Quasi_Mode;
	P03_Quasi_Mode;
	P04_Quasi_Mode;
	P05_Quasi_Mode;
	
	P20_Quasi_Mode;
	P21_Quasi_Mode;
	P24_Quasi_Mode;
}

uint8_t mKeyVal=0, mkeyValBk=0;

uint8_t mKEY_EVENT=0, mKEY_EVENT_BK=0;	// BK用于处理释放

// 按键按下，短按和长按都会经过这里；
void mKey_Press(void)
{
	if (mKEY_EVENT == 1)
		{
		if (uc_work_mode == MODE_SLEEP)
			{
			PowerUp();
			}
		else
			{
			PowerSleep();
			}
		
	//	.printf("K1");
		}
	else
		{
		// davidd 20260128, 水量键只有工作模式才能调节
		if (uc_work_mode > MODE_SLEEP)
			{
			if (mKEY_EVENT == 2)
				{
				uc_work_mode = MODE_1;
				LED_MODE1_ON; LED_MODE2_OFF; LED_MODE3_OFF; 
				//	.printf("K2");
				}
			else if (mKEY_EVENT == 3)
				{
				uc_work_mode = MODE_2;
				LED_MODE1_OFF; LED_MODE2_ON; LED_MODE3_OFF; 
				// .printf("K4");
				}
			else if (mKEY_EVENT == 4)
				{
				uc_work_mode = MODE_3;
				LED_MODE1_OFF; LED_MODE2_OFF; LED_MODE3_ON; 
				// .printf("K5");
				}
			}
		}
	
	mKEY_EVENT = 0;
}

void mKey_LongPress(void)
{
	// davidd 20260128, 转动按键 只有工作模式才能调节
	if (uc_work_mode > MODE_SLEEP)
		{
		if (mKEY_EVENT == 5)
			{
			// 左转任意角度
			}
		else if (mKEY_EVENT == 6)
			{
			// 右转任意角度		
			}
		else if (mKEY_EVENT == 7)
			{
			// 左转180度
			}
		else if (mKEY_EVENT == 8)
			{
			// 右转180度
			}
		else if (mKEY_EVENT == 3)
			{
			uc_work_mode = MODE_2;
			LED_MODE1_OFF; LED_MODE2_ON; LED_MODE3_OFF; 
			// 一直喷水消息
			
			}
		else if (mKEY_EVENT == 4)
			{
			uc_work_mode = MODE_3;
			LED_MODE1_OFF; LED_MODE2_OFF; LED_MODE3_ON; 
			// 一直喷水消息
			}
		}

}

void mKey_LongHold(void)
{

}

void mKey_Rel(void)
{

}

#if 0
void mkey_Short_Rel(void)
{

}

void mkey_Long_Rel(void)
{

}
#endif

#define KEY_DOWN 0
void mKey_Read(void)
{
	if (IN_K1_PIN == KEY_DOWN)
		{
		mKeyVal = 1;
		}
	else if (IN_K2_PIN == KEY_DOWN)
		{
		mKeyVal = 2;
		}
	else if (IN_K3_PIN == KEY_DOWN)
		{
		mKeyVal = 3;
		}
	else if (IN_K4_PIN == KEY_DOWN)
		{
		mKeyVal = 4;
		}
	else if (IN_LEFT_ADJ == KEY_DOWN)
		{
		mKeyVal = 5;
		}
	else if (IN_RIGHT_ADJ == KEY_DOWN)
		{
		mKeyVal = 6;
		}
	else if (IN_LEFT_180 == KEY_DOWN)
		{
		mKeyVal = 7;
		}
	else if (IN_RIGHT_180 == KEY_DOWN)
		{
		mKeyVal = 8;
		}
	else
		{
		mKeyVal = 0;
		}
}

#define KEY_CK_TMO		3		// 20ms检测1次；			
#define KEY_DOWN_CNT	3		// 20ms*3=60ms 按下抗抖		
#define KEY_LONG_CNT	100		// 20ms*100=2S 	长按 
#define KEY_HOLD_CNT	50		// 20ms*50=1S 	HOLD 
#define KEY_REL_CNT		2		// 20ms*2=40ms 放开抗抖	

uint8_t mKEY_CNT=0, mKEY_RELCNT=0;

void mKey_Hand(void)
{
//	if (SENS_PIN) return;	// 手柄推上去以后，不允许按键； davidd 20260128
	
	mKey_Read();

	// 按键抗抖
	
	if (mKeyVal != 0)	// 如果有值但数字不同, 计数清0, 可以兼顾组合键
		{
		if (mKeyVal == mkeyValBk)	// 如果相同,计数+1
			{
			mKEY_CNT++;	
			mKEY_RELCNT=0;	
			}
		else
			{
			// 记录当前逻辑电平, 并且原键值直接放开
			mkeyValBk = mKeyVal; f_mKeyDown = 0;
			
			mKEY_CNT=0; 
			mKEY_RELCNT=0;
			}
		}
	else	// 数值为0
		{
		mkeyValBk = 0;
		
		mKEY_CNT=0;	
		mKEY_RELCNT++;
		}

	if (f_mKeyDown == 0)
		{
		if (mKEY_CNT < KEY_DOWN_CNT)
			return;
		f_mKeyDown = 1;	
		// 短按按下执行，不需要这样的功能
		
		mKEY_EVENT = mKeyVal;
		mKey_Press();
		}
	else				// b_mKeyDown = 1;
		{	
		// 长按的处理
		if (f_mKeyLong == 0)
			{
			if (mKEY_CNT >= KEY_LONG_CNT)
				{
				mKEY_CNT = 0;
				
				// 如果一直按着不松手，不能连续执行；
				f_mKeyLong = 1;
				// 长按执行 
				mKEY_EVENT = mKeyVal;
				mKey_LongPress();
				return;
				}
			}
		else
			{
			if (mKEY_CNT >= KEY_HOLD_CNT)
				{
				mKEY_CNT = 0;
				
				// 如果一直按着不松手，可以连续执行；	
				// 长按保持执行 
				mKEY_EVENT = mKeyVal;
				mKey_LongHold();
				return;
				}
			}
		
		// 放开的处理
		if (mKEY_RELCNT >= KEY_REL_CNT)	
			{
			mKEY_RELCNT = 0; 
			f_mKeyDown = 0;
			if (f_mKeyLong == 0)
				{
				// 短按放开，执行
				// Key_ShortRel();
				}
			else
				{
				f_mKeyLong = 0;
				// 长按放开，执行
				}
			}			
		}

}

