
/**********************************************************/
/* stm8s_key.h file
/* 
/* Copyright (c) 2013 davidd
/*
/***********************************************************/
#ifndef __KEY_H__
#define __KEY_H__
/* Includes ----------------------------------------------------------------*/

/* Exported variables ------------------------------------------------------- */
#define	KEY_EVENT_BASE_ADDR			0xA0

//#define	KEY_EVENT_USER_BASE_ADDR	0xFC
//define key event.
typedef enum _KEY_EVENT
{
	IN_KEY_NONE = 0x00,
	IN_KEY_PWR_SP = KEY_EVENT_BASE_ADDR,
	IN_KEY_PWR_SPR,
	IN_KEY_PWR_CP,
	IN_KEY_PWR_CPH,
	IN_KEY_PWR_CPR,

	IN_KEY_WATER_STOP,
	IN_KEY_WATER_SMAL,
	IN_KEY_WATER_BIG,
	IN_KEY_WATER_CONT,
	IN_KEY_WATER_CONT_REL,
	
	IN_KEY_L_ADJ,
	IN_KEY_R_ADJ,
	IN_KEY_L_180,
	IN_KEY_R_180,
	IN_KEY_MOT_STOP
} KEY_EVENT;

typedef enum _USER_KEY_STATE
{
	USER_KEY_STATE_IDLE=0,
	USER_KEY_STATE_JITTER,
	USER_KEY_STATE_PRESS_DOWN,
	USER_KEY_STATE_CP,
	USER_KEY_STATE_SCP
} USER_KEY_STATE;

extern USER_KEY_STATE	UserKeyState;
/* Exported functions ------------------------------------------------------- */
KEY_EVENT GetGpioKeyEvent(void);
void GpioKeyHandle(void);
void InitKeyGpio(void);
#endif


