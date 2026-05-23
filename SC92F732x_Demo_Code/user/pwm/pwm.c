/**********************************************************/
/* pwm.c file
/* 
/* Copyright (c) 2013 davidd
/*
/***********************************************************/
/* Private define ------------------------------------------------------------*/
#define __PWM__

/* -------------------------------Includes --------------------------*/
#include "global.h"

/* -------------------------------variables --------------------------*/

/***********************************************************************/
#if 1
/*------------------------Pwm Function----------------------------*/
void Init_PwmIo(void)
{
	P00_PushPull_Mode; PWM0_HIGH;
	P01_PushPull_Mode; PWM1_HIGH;
}

void Pwm_Start(void)
{
	// 周期PWMPRD在pwm_init中设置
	// bit3: Pwm0
	// bit4: pwm1
	// bit5: pwm2
	PWMCON  = (0x18|0x05);		// PWM0输出到I/O，Fsys频率/64; Pwm频率1.88K
	PWMCON |= 0x80;				//启动PWM工作
}

// 转盘的电机
void Raft_Max(void)
{
	PWMCON  = 0;		// PWM0\PWM1输出到I/O，Fsys频率;
	PWM0_LOW; PWM1_LOW;
}

void Raft_Min(void)
{
	PWMCON  = 0;		// PWM0\PWM1输出到I/O，Fsys频率;
	PWM0_HIGH; PWM1_HIGH;
}

void Set_PwmDuty(uint8_t duty)
{
	if (duty == 0)
		{
		Raft_Min();
		}
	else if (duty == 100)
		{
		Raft_Max();
		}
	else
		{
		PWMDTY0 = duty;
		PWMDTY1 = duty;
		Pwm_Start();
		}
}

#endif

