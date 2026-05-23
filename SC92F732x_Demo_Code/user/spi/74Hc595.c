/**********************************************************/
/* 74Hc595.c file
/* ExIo
/* Copyright (c) 2002-2005 davidd
/*
/***********************************************************/

#include "global.h"
/* Private define ------------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

//********************************************************/              

/*******************************************************************************
* Function Name  : I2C_delay
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void Hc595_delay(uint16_t cnt)
{
	while(cnt--);
}

void Init_Hc595(void)
{
	P10_PushPull_Mode;
	P11_PushPull_Mode;
	P12_PushPull_Mode;

	HC_595_ALL_L;
}

void Hc595_Data_Send(uint8_t uData)
{
	uint8_t i;
	HC_595_RCLK_L;
	// SCLK L时, 改变DIO, SCLK H时, DIO数据有效
	for(i=0;i<8;i++)
		{
		HC_595_SCLK_L;
		if (uData&0x01) 
			HC_595_DIO_H; 
		else 
			HC_595_DIO_L;
		uData>>=1;
		Hc595_delay(1);
		HC_595_SCLK_H;
		Hc595_delay(1);
		}

	HC_595_SCLK_L;
	Hc595_delay(1);
	// RCLK L时, 数据改变不输出到IO, RCLK H时, DIO数据输出
	HC_595_RCLK_H;
	Hc595_delay(1);
	HC_595_RCLK_L;
}

void Hc595_Buf_Send(uint8_t *Buf, uint8_t num)
{
	uint8_t i,Data;
	unsigned char j;
  	HC_595_RCLK_L;
 	Hc595_delay(1);
	for(j=1;j<=num;j++)
		{
		// 串行输出,先送最后一个字节
		Data = *(Buf+num-j);	
		for(i=0;i<8;i++)
			{
			HC_595_SCLK_L;
			if (Data&0x01) 
				HC_595_DIO_H; 
			else 
				HC_595_DIO_L;
			Data>>=1;
			Hc595_delay(1);
			HC_595_SCLK_H;
			Hc595_delay(1);
			}
		}
	HC_595_SCLK_L;
	Hc595_delay(1);
	// RCLK L时, 数据改变不输出到IO, RCLK H时, DIO数据输出
	HC_595_RCLK_H;
	Hc595_delay(1);
	HC_595_RCLK_L;
}


