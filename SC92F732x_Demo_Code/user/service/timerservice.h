
/**********************************************************/
/* timerservice.h file
/* 
/* Copyright (c) 2013 davidd
/*
/***********************************************************/

#ifndef __TIMERSERVICE_H__
#define __TIMERSERVICE_H__
/* Includes ----------------------------------------------------------------*/

/* Exported variables ------------------------------------------------------- */

//**************below variable and define is for Timer****************************/
// used in the TIM4 ovf interrupt


//**************below variable  is for MODE ****************************/
typedef struct _PwmSetting
{
	unsigned char uSysState;			// 1 /*bit0: 开/关	bit1: 串口打印开关  bit2: 蜂鸣器开关 bit3: nvram计数开关*/
	unsigned char uWorkMode;			// 2 /*0: Body	1: Body2  2: Face	3: Eye*/
	

}PwmSetting;

extern PwmSetting gPwmSetting;


//**************below variable and define is for INPUT ****************************/

extern uint8_t idata uc_work_mode;

/* Exported functions ------------------------------------------------------- */

void ScheduleHandle(void);
void PowerUp(void);
void PowerOff(void);
void PowerSleep(void);
void show_Water_mode(uint8_t u_mode);

void Reset_para_wakeup(void);

void BeepStart(unsigned char uTimeOut,unsigned char uTimes);

void Clr_sys_Para(void);


#endif
