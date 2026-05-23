#include "global.h"

void ADC_Init(uint Channel);

bit AdcFlag = 0;


#if 0
unsigned int ADCValue = 0x0000;
/*****************************************************
*函数名称：void ADC_Test(void)
*函数功能：ADC测试
*入口参数：void
*出口参数：void
*****************************************************/
void ADC_Test(void)
{
	ADC_Init(AIN0);
	while(1)
	{
		ADCCON |= 0x40;   //开始ADC转换
		while(!AdcFlag);	         		 //等待 ADC转换完成;
		AdcFlag = 0;
		ADCValue = (ADCVH<<4)+(ADCVL>>4);
	}
}
#endif
/*****************************************************
*函数名称：void ADC_Init(uint Channel)
*函数功能：ADC初始化
*入口参数：Channel
*出口参数：void
*****************************************************/
void ADC_Init(uint Channel)
{
	ADCCON = 0x80|Channel;			//开启ADC，ADC采样频率为2M ,选择Channel位ADC采样口
	if(Channel<8)
	{
		ADCCFG0 = 1<<Channel;   	//设置Channel作为采样口EAINx
	}
	else
	{
		ADCCFG1 = 1<<(Channel-8);   //设置Channel作为采样口
	}

//	IE |= 0x40;        //开启ADC中断
//	EA = 1;
}


void ADC_Interrupt(void) interrupt 6
{
	ADCCON &= ~(0x10);  //清中断标志位
	AdcFlag = 1;
}



