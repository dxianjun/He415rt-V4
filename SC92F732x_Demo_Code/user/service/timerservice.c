/**********************************************************/
/* timerservice.c file
/* 
/* Copyright (c) 2013 davidd
/*
/***********************************************************/
/* Private define ------------------------------------------------------------*/
#define __TIMERSERVICE__

/* -------------------------------Includes --------------------------*/
#include "global.h"


/* -------------------------------variables --------------------------*/
/***********************************************************************/
// bool
// unsigned char
// volatile bool

bit_field_t data B_Sys_Flag, B_Sys_Flag2;
// uint8_t idata  uc_waterSet=2;
uint8_t idata uc_work_mode=0;
// u_workState: 0-off; 1-chg, 2-dsg, 3-chg+dsg; 
// uint8_t idata  u_workState=0;

/* -------------------------------Function --------------------------*/

/* ---------------------- hardward function-------------------------*/
/*------------------------IO Function----------------------------*/

// 执行关机时, 把状态和计数全部清0;
void Clr_sys_Para(void)
{
	B_Sys_Flag.val = 0;
	B_Sys_Flag2.val = 0;
}

void Reset_para_wakeup(void)
{
// 唤醒后, 复位全局变量
	B_Sys_Flag.val = 0;
	B_Sys_Flag2.val = 0;

	uc_work_mode = 0;
	UserKeyState = USER_KEY_STATE_IDLE;
}

void show_Water_mode(uint8_t u_mode)
{
	switch (u_mode)
		{
		default: break;

		case MODE_1:
			{
						
			LED_SLEEP_OFF;					
			LED_WATER_STOP_ON; LED_MODE3_OFF; 
			}
			break;

		case MODE_2:
			{			
			LED_SLEEP_OFF;					
			LED_WATER_SMALL_ON; LED_MODE3_OFF; 
			}
			break;

		case MODE_3:
			{		
			LED_SLEEP_OFF;					
			LED_WATER_BOTH_OFF; LED_MODE3_ON; 
			}
			break;
		}
}

void PowerUp(void)
{
	uc_work_mode = MODE_3;			// 开机默认大水量
	LED_SLEEP_OFF;					// 按开机键后，进入工作状态，此时待机灯要灭
	LED_WATER_BOTH_OFF; LED_MODE3_ON; 	// 从sleep到工作状态，亮对应水量灯；

	Send_Info_power(uc_work_mode);	//B6 01 01 02~04 FF
}

void PowerSleep(void)
{
	uc_work_mode = MODE_SLEEP;	
	// PWR_CTRL = PWR_ON;			// 上电，但是没有按开机键时的状态，定义为待机状态
	
	LED_SLEEP_ON;			
	LED_WATER_BOTH_OFF; LED_MODE3_OFF;		// 从开机到关机时，要把水量灯灭掉

	Send_Info_power(uc_work_mode);	//B6 01 01 01 FF
}

void PowerOff(void)
{
	uc_work_mode = MODE_OFF;
	LED_SLEEP_OFF;LED_WATER_BOTH_OFF; LED_MODE3_OFF;

	Send_Info_power(uc_work_mode);	//B6 01 01 00 FF
	
	// 断电源
	// PWR_CTRL = PWR_OFF;
	
#if 1		
	WaitWakeUp();
#endif	
	// 唤醒后, 复位全局变量
	Reset_para_wakeup();

#if 0
	// 没有唤醒功能，只能断电
	while(1)
		{
		WDG_ReloadCounter;
		}
#endif
}

#if 1
uint8_t Is_Sens_ok(void)
{
	if (SENS_PIN) 
		return 0;
	else
		return 1;
}

// 当前状态与上一个稳态相同, 计数加1, 累计够几次,就置变量,并执行动作;
// 这种写法, 要3个全局变量, 以防止事件重复执行;
// 注意:这3个变量从待机唤醒清0;
uint8_t idata uc_Sens_ok_cnt=0;  
void Sens_In_Hand(void)
{
	if (Is_Sens_ok())
		{
		uc_Sens_ok_cnt++;		// 20ms*15=0.3S	
		if (uc_Sens_ok_cnt == 15)
			{
			// 进入待机
			if (!f_Sens_ok)
				{
				f_Sens_ok = 1; 
				if (uc_work_mode == MODE_OFF)
					PowerSleep();	// 等待按键开机
				}
			}
		else if (uc_Sens_ok_cnt > 15)		
			{
			uc_Sens_ok_cnt = 16;		// 固定死，防止溢出后，重复执行
			}
		}
	else
		{
		// davidd add 20260206, 增加污水满检测的减值，解决间断触发时，累加会误触发污水满；
		if (f_Sens_ok)
			{
			uc_Sens_ok_cnt--;
			if (uc_Sens_ok_cnt == 0)
				{
				if (uc_work_mode == MODE_SLEEP)
					{
					PowerOff();
					}
				}
			}
		else
			{
			uc_Sens_ok_cnt=0;
			}
		}
}

#endif

#define PWR_SLEEP	1

#if PWR_SLEEP
void WaitPowerOff(void)
{
	// 唤醒后1S, 如果没有充电也没有放电, 就待机
	if (TimOut10mS[TCKOFF] >= 100)
		{
		TimOut1mS[TCKOFF] = 0;
		// if (u_workState==WORK_OFF)		// 只有工作模式才需要定时关机；
		if (uc_work_mode == MODE_OFF)
			{
			// 误触后，如果没有按键，1S定时到后关机；
			if (SENS_PIN == 1)
				{
				PowerOff();	
				}
			return;
			}
		}
}
#endif

// davidd add 20250428, 增加喷水时长调节, 0.5, 0.8. 1.0秒,  50即0.5秒;
// 2是大水量, 1是小水量,0是停止，3是一直喷


void InputHandle(void)
{
	if (TimOut1mS[TINP] < 20)
		return;	
	TimOut1mS[TINP] = 0;

	Sens_In_Hand();
	// 按键读取
	GpioKeyHandle();
}

void ScheduleHandle(void)
{
	WDG_ReloadCounter;
	InputHandle();
	#if PWR_SLEEP	// 添加待机唤醒的功能，现在先不用；
	WDG_ReloadCounter;
	WaitPowerOff();
	#endif
	WDG_ReloadCounter;
	Tcp_Hand();
}


