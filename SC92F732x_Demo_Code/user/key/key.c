/**********************************************************/
/* key.c file   for SC92F7322
/* 
/* Copyright (c) 2013 davidd
/*
/***********************************************************/
/* Private define ------------------------------------------------------------*/
#define __SC92_KEY__

/* -------------------------------Includes --------------------------*/
#include "global.h"

/* -------------------------------variables --------------------------*/

/***********************************************************************/
USER_KEY_STATE	UserKeyState = USER_KEY_STATE_IDLE;

#define NONE_SCPS	1

#if NONE_SCPS
static unsigned char GpioKeyEvent[][5] = 
{
// SP 				SPR						CP 					CPH						CPR
//short press		short release,		Long press			hold,continue key	Long press release
	{IN_KEY_PWR_SP,	IN_KEY_NONE,		IN_KEY_PWR_CP, 		IN_KEY_NONE, 		IN_KEY_NONE},
	{IN_KEY_WATER_STOP,	IN_KEY_NONE,		IN_KEY_NONE, 		IN_KEY_NONE, 		IN_KEY_NONE},
	{IN_KEY_WATER_SMAL,	IN_KEY_NONE,		IN_KEY_WATER_CONT, 		IN_KEY_NONE, 		IN_KEY_WATER_CONT_REL},
	{IN_KEY_WATER_BIG,	IN_KEY_NONE,		IN_KEY_WATER_CONT, 		IN_KEY_NONE, 		IN_KEY_WATER_CONT_REL},

	{IN_KEY_L_ADJ,	IN_KEY_MOT_STOP,		IN_KEY_NONE, 		IN_KEY_NONE, 		IN_KEY_MOT_STOP},
	{IN_KEY_R_ADJ,	IN_KEY_MOT_STOP,		IN_KEY_NONE, 		IN_KEY_NONE, 		IN_KEY_MOT_STOP},
	{IN_KEY_L_180,	IN_KEY_NONE,		IN_KEY_NONE, 		IN_KEY_NONE, 		IN_KEY_NONE},
	{IN_KEY_R_180,	IN_KEY_NONE,		IN_KEY_NONE, 		IN_KEY_NONE, 		IN_KEY_NONE},
};

#elif 0
static unsigned char GpioKeyEvent[][6] = 
{
// SP 				SPR						CP 				SCPS				CPH						CPR
//short press		short release,		Long press			super long press	hold,continue key	Long press release
	{IN_KEY_PWR_SP,	IN_KEY_NONE,		IN_KEY_PWR_CP, 		IN_KEY_NONE,		IN_KEY_NONE, 		IN_KEY_NONE},
	{IN_KEY_WATER_STOP,	IN_KEY_NONE,		IN_KEY_NONE, 		IN_KEY_NONE,		IN_KEY_NONE, 		IN_KEY_NONE},
	{IN_KEY_WATER_SMAL,	IN_KEY_NONE,		IN_KEY_WATER_CONT, 		IN_KEY_NONE,		IN_KEY_NONE, 		IN_KEY_WATER_CONT_REL},
	{IN_KEY_WATER_BIG,	IN_KEY_NONE,		IN_KEY_WATER_CONT, 		IN_KEY_NONE,		IN_KEY_NONE, 		IN_KEY_WATER_CONT_REL},

	{IN_KEY_L_ADJ,	IN_KEY_MOT_STOP,		IN_KEY_NONE, 		IN_KEY_NONE,		IN_KEY_NONE, 		IN_KEY_MOT_STOP},
	{IN_KEY_R_ADJ,	IN_KEY_MOT_STOP,		IN_KEY_NONE, 		IN_KEY_NONE,		IN_KEY_NONE, 		IN_KEY_MOT_STOP},
	{IN_KEY_L_180,	IN_KEY_NONE,		IN_KEY_NONE, 		IN_KEY_NONE,		IN_KEY_NONE, 		IN_KEY_NONE},
	{IN_KEY_R_180,	IN_KEY_NONE,		IN_KEY_NONE, 		IN_KEY_NONE,		IN_KEY_NONE, 		IN_KEY_NONE},
};

#endif
/* -------------------------------Function --------------------------*/
void InitKeyGpio(void)
{
	// 输入上拉
	P00_Quasi_Mode;
	P01_Quasi_Mode;
	P04_Quasi_Mode;
	P05_Quasi_Mode;
	
	P20_Quasi_Mode;
	P21_Quasi_Mode;
	P24_Quasi_Mode;
	P25_Quasi_Mode;
}

#define KEY_DOWN 0
unsigned char GetGpioKeyIndex(void)
{
	unsigned char idata uKeyIndex=0;
	if (IN_K1_PIN == KEY_DOWN)
		{
		uKeyIndex = 1;
		}
	else if (IN_K2_PIN == KEY_DOWN)
		{
		uKeyIndex = 2;
		}
	else if (IN_K3_PIN == KEY_DOWN)
		{
		uKeyIndex = 3;
		}
	else if (IN_K4_PIN == KEY_DOWN)
		{
		uKeyIndex = 4;
		}
	else if (IN_LEFT_ADJ == KEY_DOWN)
		{
		uKeyIndex = 5;
		}
	else if (IN_RIGHT_ADJ == KEY_DOWN)
		{
		uKeyIndex = 6;
		}
	else if (IN_LEFT_180 == KEY_DOWN)
		{
		uKeyIndex = 7;
		}
	else if (IN_RIGHT_180 == KEY_DOWN)
		{
		uKeyIndex = 8;
		}
	else
		{
		uKeyIndex = 0;
		}
	
	return uKeyIndex;
}

KEY_EVENT GetGpioKeyEvent(void)
{
	static unsigned char uPreKeyIndex = 0;
	unsigned char uKeyIndex=0;

	uKeyIndex = GetGpioKeyIndex();
	switch (UserKeyState)
		{
		case USER_KEY_STATE_IDLE:
			{
			if(uKeyIndex == 0)
				{
				return IN_KEY_NONE;
				}
			
			uPreKeyIndex = uKeyIndex;
			TimOut10mS[TKEYWAIT] = 0;	// 
			UserKeyState = USER_KEY_STATE_JITTER;
			}
			break;
		case USER_KEY_STATE_JITTER:
			{
			if(uPreKeyIndex != uKeyIndex)
				{
				UserKeyState = USER_KEY_STATE_IDLE;
				}
			else if(TimOut10mS[TKEYWAIT]>=TIMOUT_KEYJITTER_10MS)
				{
 			    TimOut10mS[TKEYWAIT] = 0;	// 
				UserKeyState = USER_KEY_STATE_PRESS_DOWN;
				return GpioKeyEvent[uPreKeyIndex-1][0];
				}
			}
			break;

		case USER_KEY_STATE_PRESS_DOWN:
			{
			if(uPreKeyIndex != uKeyIndex)
				{
				//return key sp value
				UserKeyState = USER_KEY_STATE_IDLE;
				return GpioKeyEvent[uPreKeyIndex-1][1];
				}
			else if(TimOut10mS[TKEYWAIT]>=TIMOUT_KEYCP_10MS)
				{
 			    TimOut10mS[TKEYWAIT] = 0;	// 
				UserKeyState = USER_KEY_STATE_CP;
				//return key cps value
				return GpioKeyEvent[uPreKeyIndex-1][2];						
				}
			}
			break;

		#if NONE_SCPS	// 不需要超长按的
		case USER_KEY_STATE_CP:
			{
			if(uPreKeyIndex != uKeyIndex)
				{
				//return key cps value
				UserKeyState = USER_KEY_STATE_IDLE;
				return GpioKeyEvent[uPreKeyIndex-1][4];
				}
			else if(TimOut10mS[TKEYWAIT]>=TIMOUT_KEYCPH_10MS)
				{
 			    TimOut10mS[TKEYWAIT] = 0;	// 
				//return key cph value
				return GpioKeyEvent[uPreKeyIndex-1][3];				
				}
			}
			break;
		#elif 0	// 需要超长按的
		case USER_KEY_STATE_CP:
			{
			if(uPreKeyIndex != uKeyIndex)
				{
				//return key cps value
				UserKeyState = USER_KEY_STATE_IDLE;
				return GpioKeyEvent[uPreKeyIndex-1][5];
				}
			else if(TimOut10mS[TKEYWAIT]>=TIMOUT_KEYSCP_10MS)
				{
 			    TimOut10mS[TKEYWAIT] = 0;	// 
 			    UserKeyState = USER_KEY_STATE_SCP;
				//return key scp value
				return GpioKeyEvent[uPreKeyIndex-1][3];						
				}
			}
			break;
			
		case USER_KEY_STATE_SCP:
			{
			if(uPreKeyIndex != uKeyIndex)
				{
				//return key cpr value
				UserKeyState = USER_KEY_STATE_IDLE;
				return GpioKeyEvent[uPreKeyIndex-1][5];
				}
			else if(TimOut10mS[TKEYWAIT]>=TIMOUT_KEYCPH_10MS)
				{
 			    TimOut10mS[TKEYWAIT] = 0;	// 
				//return key cph value
				return GpioKeyEvent[uPreKeyIndex-1][4];						
				}
			}
			break;
		#endif

		default:
			UserKeyState = USER_KEY_STATE_IDLE;
			break;
		}
	return IN_KEY_NONE;
}

#define PAIR_SEQ_TIMEOUT_10MS 200
#define PAIR_STEP_NONE        0
#define PAIR_STEP_STOP        1
#define PAIR_STEP_SMAL        2

uint8_t idata uc_pair_key_step = PAIR_STEP_NONE;

uint8_t PairKeySeqHandle(KEY_EVENT GpioKeyEvent)
{
	if (uc_work_mode <= MODE_SLEEP)
		{
		uc_pair_key_step = PAIR_STEP_NONE;
		return 0;
		}

	if ((uc_pair_key_step != PAIR_STEP_NONE) && (TimOut10mS[TPAIR_SEQ] > PAIR_SEQ_TIMEOUT_10MS))
		{
		uc_pair_key_step = PAIR_STEP_NONE;
		}

	switch (GpioKeyEvent)
		{
		default:
			break;

		case IN_KEY_WATER_STOP:
			uc_pair_key_step = PAIR_STEP_STOP;
			TimOut10mS[TPAIR_SEQ] = 0;
			break;

		case IN_KEY_WATER_SMAL:
			if ((uc_pair_key_step == PAIR_STEP_STOP) && (TimOut10mS[TPAIR_SEQ] <= PAIR_SEQ_TIMEOUT_10MS))
				{
				uc_pair_key_step = PAIR_STEP_SMAL;
				}
			else
				{
				uc_pair_key_step = PAIR_STEP_NONE;
				}
			break;

		case IN_KEY_WATER_BIG:
			if ((uc_pair_key_step == PAIR_STEP_SMAL) && (TimOut10mS[TPAIR_SEQ] <= PAIR_SEQ_TIMEOUT_10MS))
				{
				uc_pair_key_step = PAIR_STEP_NONE;
				Pairing_StartReadMac();
				return 1;
				}
			else
				{
				uc_pair_key_step = PAIR_STEP_NONE;
				}
			break;
		}
	return 0;
}

void GpioKeyHandle(void)	
{
	KEY_EVENT GpioKeyEvent=IN_KEY_NONE;
	GpioKeyEvent = GetGpioKeyEvent();
	if (GpioKeyEvent != IN_KEY_NONE)
		{
		if (PairKeySeqHandle(GpioKeyEvent))
			{
			return;
			}
	#if 1
		switch (GpioKeyEvent)
			{
			default: break;
			
			case IN_KEY_PWR_SP:
				{
				if (MODE_SLEEP == uc_work_mode)
					{
					PowerUp();				
					}
				else
					{
					PowerSleep();		
					}
				}
				break;
				
			case IN_KEY_WATER_STOP:
				{
				if (uc_work_mode > MODE_SLEEP)
					{
					uc_work_mode = MODE_1;
					show_Water_mode(uc_work_mode);
					
					// 送停止喷水消息
					Send_Info_waterSet(WATER_STOP);
					}
				}
				break;

			case IN_KEY_WATER_SMAL:
				{
				if (uc_work_mode > MODE_SLEEP)
					{
					uc_work_mode = MODE_2;
					show_Water_mode(uc_work_mode);
					
					// 送小水量消息
					Send_Info_waterSet(WATER_SMAL);
					}
				}
				break;
			
			case IN_KEY_WATER_BIG:
				{
				if (uc_work_mode > MODE_SLEEP)
					{
					uc_work_mode = MODE_3;
					show_Water_mode(uc_work_mode);
					// 送大水量消息
					Send_Info_waterSet(WATER_BIG);
					}
				}
				break;	

			case IN_KEY_WATER_CONT:
				{
				if (uc_work_mode > MODE_SLEEP)
					{
					// 送持续喷消息
					Send_Info_waterSet(WATER_CONT);
					}
				}
				break;	

			case IN_KEY_WATER_CONT_REL:
				{
				if (uc_work_mode > MODE_SLEEP)
					{
					// 送持续喷结束消息
					Send_Info_waterSet(WATER_CONT_REL);
					}
				}
				break;
			
			case IN_KEY_L_ADJ:
				{
				if (uc_work_mode > MODE_SLEEP)
					{
					// 送持续左转消息
					Send_Info_MotSet(MOT_L_CONT);
					}
				}
				break;	

			case IN_KEY_R_ADJ:
				{
				if (uc_work_mode > MODE_SLEEP)
					{
					// 送持续右转消息
					Send_Info_MotSet(MOT_R_CONT);
					}
				}
				break;
			case IN_KEY_L_180:
				{
				if (uc_work_mode > MODE_SLEEP)
					{
					// 送左转消息180
					Send_Info_MotSet(MOT_L_180);
					}
				}
				break;	

			case IN_KEY_R_180:
				{
				if (uc_work_mode > MODE_SLEEP)
					{
					// 送右转消息180
					Send_Info_MotSet(MOT_R_180);
					}
				}
				break;
			
			case IN_KEY_MOT_STOP:
				{
				if (uc_work_mode > MODE_SLEEP)
					{
					// 送停止旋转消息
					
					Send_Info_MotSet(MOT_STOP);
					}
				}
				break;
				
			}
	#endif
		}
}













