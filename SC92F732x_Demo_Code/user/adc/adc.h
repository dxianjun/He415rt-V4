
/**********************************************************/
/* adc.h file
/* 
/* Copyright (c) 2013 davidd
/*
/***********************************************************/
#ifndef __ADC_H__
#define __ADC_H__

/* Includes ----------------------------------------------------------------*/

/* public defines -----------------------------------------------------------*/
/***********ADC采样口选择*************/
#if (IC_MODEL == SC92F7323)
	enum Channel {AIN0=0,AIN1,AIN2,AIN3,AIN4,AIN5,AIN6,AIN7,AIN8,AIN9,VDD4=15};
#endif

#if (IC_MODEL == SC92F7322)
	enum Channel {AIN0=0,AIN1,AIN4=4,AIN5,AIN6,AIN7,AIN8,AIN9,VDD4=15};
#endif
	
#if (IC_MODEL == SC92F7321)
	enum Channel {AIN0=0,AIN1,AIN2,AIN3,AIN8=8,AIN9,VDD4=15};
#endif
	
#if (IC_MODEL == SC92F7320)
	enum Channel {AIN0=0,AIN1,AIN6=6,AIN7,VDD4=15};
#endif
	
/***********ADC采样口选择*************/
#if (IC_MODEL == SC92F7352)
	enum Channel {AIN0=0,AIN1,AIN4=4,AIN5,AIN6,AIN7,AIN8,AIN9,VDD4=15};
#endif

#if (IC_MODEL == SC92F7351)
	enum Channel {AIN0=0,AIN1,AIN4=4,AIN5,AIN8=8,AIN9,VDD4=15};
#endif
	
#if (IC_MODEL == SC92F7350)
	enum Channel {AIN0=0,AIN1,AIN6=6,AIN7,VDD4=15};
#endif	


extern bit b_DcIn;
extern bit b_BatLow;
extern bit b_BatFull, b_BatOver;

extern uint8_t idata u_BatState;
extern uint8_t idata u_duty, u_dutyBk;
extern uint16_t idata ui_warnLed_dly;
extern uint8_t idata uc_On_times;

/* Exported functions ------------------------------------------------------- */
void Init_AdcIo(void);
void Init_BatState(void);
void Adc_Hand(void);

void Get_PwmDuty(void);
#endif

