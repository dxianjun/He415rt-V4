#ifndef __74HC595_Driver_H__
#define __74HC595_Driver_H__

/* Includes ------------------------------------------------------------------*/

#define HC595_NUM			1

#define HC595_OE_PIN		// 直接接地了; GPIO_Pin_4							// 13Pin
#define HC595_SHCP_PIN		P10			// SCK--Shiftclock			11Pin
#define HC595_STCP_PIN		P11			// RCK--StoreClock			12Pin
#define HC595_DS_PIN		P12			// DATA						14Pin
// 各个厂家pin脚名称不同, 功能是一样的;

#define HC_595_SCLK_L	HC595_SHCP_PIN=0 
#define HC_595_SCLK_H	HC595_SHCP_PIN=1 
#define HC_595_DIO_L	HC595_DS_PIN=0
#define HC_595_DIO_H  	HC595_DS_PIN=1
#define HC_595_RCLK_L  	HC595_STCP_PIN=0
#define HC_595_RCLK_H  	HC595_STCP_PIN=1
#define HC_595_EN     	//HC595_OE_PIN=0
#define HC_595_DIS    	//HC595_OE_PIN=1 
#define HC_595_ALL_L	{HC_595_EN;HC_595_SCLK_L;HC_595_DIO_L;HC_595_RCLK_L;}

void Init_Hc595(void);
void Hc595_Buf_Send(uint8_t *Buf, uint8_t num);
void Hc595_Data_Send(uint8_t uData);

#endif 



