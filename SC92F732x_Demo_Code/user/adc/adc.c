/**********************************************************/
/* adc.c file
/* 
/* Copyright (c) 2013 davidd
/*
/***********************************************************/
/* Private define ------------------------------------------------------------*/
#define __ADC__

/* -------------------------------Includes --------------------------*/
#include "global.h"

/* -------------------------------variables --------------------------*/

/***********************************************************************/
double idata ADC_Volt;			//please always use "double" mode for this
uint16_t ADCValue = 0;

void Init_AdcIo(void)
{
#if (IC_MODEL == SC92F7351)
	P24_Input_Mode;
#else
	P22_Input_Mode;
#endif
}

// 12bit
unsigned short Read_ADC(u8 chan)
{
	ADC_Init(chan);
	
	ADCCON |= 0x40;   							//开始ADC转换
	while ((ADCCON&0x10) == 0);	    			//等待 ADC转换完成;
	ADCCON &= ~(0x10);							//清中断标志位  				
	ADCValue = (ADCVH<<4)+(ADCVL>>4);
	
	return ADCValue;
}

uint16_t GetAdcVolt(uint8_t ch)			
{
	uint16_t idata uAdcVolt=0;
	uint8_t i=0;
	for (i=0;i<8;i++)
		uAdcVolt += Read_ADC(ch);
	
	uAdcVolt = uAdcVolt>>3;			
	return uAdcVolt;
}

uint16_t Get_BatVolt(void)
{
	unsigned short idata value=0;
	value = GetAdcVolt(ADC_BAT);
	ADC_Volt = value*5000.0/0x1000;
	return (unsigned short)ADC_Volt;
}

bit b_DcIn=0;
bit b_BatLow=0;
bit b_BatFull=0;
bit b_BatOver=0;	// davidd add 20250315

uint16_t idata w_BatVolt;
uint8_t idata u_BatState;

uint8_t idata BatPer;
uint8_t Get_BatPer(void)
{
	w_BatVolt = Get_BatVolt();

	if (w_BatVolt >= VOC_OVER)		
		BatPer = BAT_STATE_OVER;	// 充电保护
	else if (w_BatVolt > VOC100)
		BatPer = BAT_STATE_FULL;	
	else if (w_BatVolt > VOC70)
		BatPer = BAT_STATE_75;		// 75+
	else if (w_BatVolt > VOC50)
		BatPer = BAT_STATE_50;		// 50+
	else if (w_BatVolt > VOC20)
		BatPer = BAT_STATE_25;		// 25+
	else if (w_BatVolt > VOC5)
		BatPer = BAT_STATE_10;		// 10+/25-
	else
		BatPer = BAT_STATE_LOW;

	return BatPer;
}

// 电池刚接入时, 用电压识别电量, 都按放电标识
void Init_BatState(void)
{
	u_BatState = Get_BatPer();

	if (u_BatState >= BAT_STATE_FULL)
		u_BatState = BAT_STATE_75;

	if (u_BatState == BAT_STATE_LOW)
		{
		b_BatLow = 1;
		SET_INFO_HIGH; 
		// 记录次数，第一次进入为1, 后边都是为2；
		uc_On_times++;
		if (uc_On_times > 2) uc_On_times = 2;
		}
	#if 1	// davidd 20250919, 只有充电才能解锁，复位不能解锁；
	else
		{
		b_BatLow = 0;
		SET_INFO_LOW;  // 初始化时直接初始化成了Low, 可以不写;
		}
	#endif
	ui_warnLed_dly = 0;
}

void Get_PwmDuty(void);
extern bit b_IsBatFlash_On;

uint8_t idata u_duty;
uint8_t idata u_dutyBk=0;

// 充放电处理, 电量检测
void Chg_Hand(void)
{
	// Volt-->Per-->State	// davidd 20250315
	// davidd 20251017, 充电时，先把pwm关掉后再读
	if (!b_BatOver)
		{
		Set_PwmDuty(0); Delay_1ms(10);
		}
	
	Get_BatPer();
	
	if (b_BatOver)
		{
		// 解锁,续充, 20250315
		if (BatPer < BAT_STATE_FULL)	// 发现电池电压小于4.1V
			{
			b_BatOver = 0; BatPer = BAT_STATE_FULL; u_BatState = BAT_STATE_FULL; 
			// Get_PwmDuty();
			u_dutyBk = u_duty = 100;
			Set_PwmDuty(u_duty);
			return;
			}
		}
	else
		{
		// 不显示实际电压，慢慢升降 20220524
		// 充电时，只能增加
		if (b_BatFull)
			{
			// 只有小于BAT_STATE_FULL时,需要赋值, >= 时,不能改也不用改; davidd 20250315
			if (BatPer < BAT_STATE_FULL)
				BatPer = BAT_STATE_FULL;
			}
		else
			{
			// 充电时, 最小就是25-
			if (BatPer < BAT_STATE_10)
				{
				BatPer = BAT_STATE_10;
				}
			}
		
		// 这里BatPer 的范围是 [BAT_STATE_10, BAT_STATE_OVER]
		if (BatPer > u_BatState)
			{
			u_BatState = u_BatState+1;
			Show_BatFrame(u_BatState);

			if (u_BatState == BAT_STATE_OVER)
				{
				b_BatFull = 1; b_BatOver = 1;
				}
			else if (u_BatState == BAT_STATE_FULL)
				{
				b_BatFull = 1;
				}

			Get_PwmDuty();
			}

	#if 1
		// davidd 20251017, 充电时改为先停pwm，执行完判断后，重新开；
		Set_PwmDuty(u_duty);
	#else
		if (u_dutyBk != u_duty)
			{
			u_dutyBk = u_duty;
			Set_PwmDuty(u_duty);
			}
	#endif
		}
}

// davidd add for BatLow 20250225
uint16_t idata ui_warnLed_dly=0;

// 记录是否是仅提示，第一次要求闪灯60分钟，同时1分钟时，拉高P0.2通知电机模块停止工作；
// 第二次，闪灯60秒，直接关；P0.2一直处于锁死的状态；
uint8_t idata uc_On_times=0;	

void Dsg_Hand(void)
{
	Get_BatPer();

	// 放电时, 最大就是75+
	if (BatPer > BAT_STATE_75)
		BatPer = BAT_STATE_75;
	
	// 不显示实际电压，慢慢升降 20220524
	// 放电时，只能减小
	if (BatPer < u_BatState)
		{
		u_BatState = u_BatState-1;
		Show_BatFrame(u_BatState);
		
		// davidd 20220524 等到显示降到10以下，才算低电量
		if (u_BatState == BAT_STATE_LOW)
			{
			if (b_BatLow==0)
				{
				b_BatLow = 1;

				// 记录次数，第一次进入为1, 后边都是为2；
				uc_On_times++;
				if (uc_On_times > 2) uc_On_times = 2;
				
				// davidd 20250225 add for O_INFO, 低电量时输出高电平, 通知电机模块停止工作
				// SET_INFO_HIGH;

				// 定时时间开始
				ui_warnLed_dly = 0;
				}
			}
		}
}

// 定时30秒处理1次,  每次最多减1格 或 1%;
void Adc_Hand(void)
{
	if (u_workState == WORK_OFF) return;
	
	if (TimOut1S[TADC_HND] < 10)
		return;	
	TimOut1S[TADC_HND] = 0;
	
	w_BatVolt = Get_BatVolt();
	if (b_DcIn) 
		Chg_Hand();
	else
		Dsg_Hand();
		
}

void Get_PwmDuty(void)
{
	if (u_workState == WORK_CHG)
		{
		switch (u_BatState)
			{
			default: 
			case BAT_STATE_OVER:	// davidd 20250315
				u_duty = 0;
				break;

			case BAT_STATE_10:
				u_duty = 40;		// 30
				break;

			case BAT_STATE_25:
				u_duty = 70;		// 50
				break;

			case BAT_STATE_50:
				u_duty = 85;		// 75
				break;

			case BAT_STATE_75:
			case BAT_STATE_FULL:	// davidd 20250315
				u_duty = 100;
				break;
			}
		}
	else if (u_workState == WORK_CHG_DSG)
		{
		u_duty = 100;
		}
	else // 仅放电或者待机
		{
		u_duty = 0;
		}
}

