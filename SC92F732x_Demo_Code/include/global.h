/**********************************************************/
/* global.h file
/* 
/* Copyright (c) 2013 davidd
/*
/***********************************************************/
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <intrins.H>

#include "type_define.h"
#include "Function_Init.h"
#if ((IC_MODEL == SC92F7351)||(IC_MODEL == SC92F7352)||(IC_MODEL == SC92F7350))
#include "SC92F735x_c.h"
#else
#include "SC92F732x_c.h"
#endif

#include "SFR_Macro.h"
//#include "Function_define.h"

#include "SC_it.h"
#include "timer.h"
#include "timerservice.h"
#include "key.h"
//#include "ir_433.h"
//#include "adc.h"
//#include "pwm.h"
#include "uart.h"
#include "i2c_master.h"
#include "ep24cx_i2c.h"

#if 0
#include "main.h"
#include "aprom.h"
#include "gpio.h"


#endif


#define IapROM  0x00	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作
#define IapEPPROM  0x02	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作

#define EVENTMOD	1
#define POLLING		0

#define EN_INTER_EEPROM	1


// below is ADC
#define ADC_VDD				8		// Internal band-gap
#define ADC_BAT				2		// P2.2	// 电池电量检测

// below is input
// #define WATER_DET			P17		//  IPU P1.7	水位状态检测，低电平有效, IPU 
#define SENS_PIN			P10		//  IPU P1.0 	倾斜开关检测

#define IN_LEFT_ADJ			P24		// IPU 左转任意角度
#define IN_LEFT_180			P21		// IPU  左转180度
#define IN_RIGHT_ADJ		P20		// IPU  左转任意角度
#define IN_RIGHT_180		P05		// IPU  左转180度

#define IN_K4_PIN			P04		// IPU  大水量
#define IN_K3_PIN			P25		// IPU  小水量
#define IN_K2_PIN			P01		// IPU  停止
#define IN_K1_PIN			P00		// IPU  开关按键


// below is output	
#define CTRL_BT_RST			P16		// PPH，长低会让蓝牙复位
#define CTRL_LED_R			P17		// PPH 红灯，待机时亮，倾斜检测到位后，开机前
#define CTRL_LED_K4			P27		// PPH 低电平亮，K4指示灯，大水量
#define CTRL_LED_K3			P26		// P2.6 双灯复用：0亮小水量灯，1亮停止灯，输入高阻两灯灭


// below is PWM
// #define PWM_0				P00		// PWM0
// #define PWM_1				P01		// PWM1

// IO init end
#define LED_ON				0
#define LED_OFF				1

#define BT_RST_PRESS		CTRL_BT_RST=0
#define BT_RST_REL			CTRL_BT_RST=1

#define LED_SLEEP_ON		CTRL_LED_R=0
#define LED_SLEEP_OFF		CTRL_LED_R=1

#define LED_MODE3_ON		CTRL_LED_K4=0
#define LED_MODE3_OFF		CTRL_LED_K4=1

#define LED_WATER_SMALL_ON	{P26_PushPull_Mode; CTRL_LED_K3=0;}
#define LED_WATER_STOP_ON	{P26_PushPull_Mode; CTRL_LED_K3=1;}
#define LED_WATER_BOTH_OFF	{CTRL_LED_K3=1; P26_Input_Mode;}

#define LED_MODE1_ON		LED_WATER_STOP_ON
#define LED_MODE1_OFF		LED_WATER_BOTH_OFF

#define LED_MODE2_ON		LED_WATER_SMALL_ON
#define LED_MODE2_OFF		LED_WATER_BOTH_OFF


// end
#define MODE_OFF	0				// OFF
#define MODE_SLEEP	1				// 待机状态，上电了，但是没有开机
#define MODE_1		2				// 停止灯显示
#define MODE_2		3				// 小水量
#define MODE_3		4				// 大水量
#define MODE_MAX	MODE_3

#define WATER_STOP	0
#define WATER_SMAL	1
#define WATER_BIG	2
#define WATER_CONT	3
#define WATER_CONT_REL	4			// 用于持续喷水结束


#define MOT_STOP	0				// 这里只是停止旋转，不是要停止电机继续走
#define MOT_L_CONT	1
#define MOT_R_CONT	2
#define MOT_L_180	3
#define MOT_R_180	4



#if 0
#define PWM1_HIGH			PWM_1=1
#define PWM1_LOW			PWM_1=0

#define PWM0_HIGH			PWM_0=1
#define PWM0_LOW			PWM_0=0

#define PWM_MAX				100
#define	PWM_INT				0

#define DUTY_MAX			100
#define DUTY_MIN			5
#endif

#endif






