#include "global.h"


void EX_Init(void);
uchar INT2_flag = 0x00;

void EX_Init(void)
{
#if 0
	//配置中断口INT02
	P12_Quasi_Mode;		//	中断IO口设置为高阻带上拉输入
	P12 = 1;
	
	//配置INT02 下降沿中断
	//下降沿设置	
	INT0F = 0X04;    //xxxx 0000  0关闭 1使能
	//上升沿设置	
	INT0R = 0X00;    //xxxx 0000  0关闭 1使能 

	// 是能INT0中断;
	EINT0 = 1;
	IPINT0 = 0;
#endif

#if 1
	//配置中断口INT00
	P10_Quasi_Mode;		//	中断IO口设置为高阻带上拉输入
	P10 = 1;
	
	//配置INT00 下降沿中断
	//下降沿设置	
	INT0F = 0X01;    //xxxx 0000  0关闭 1使能
	//上升沿设置	
	INT0R = 0X00;    //xxxx 0000  0关闭 1使能 

	// 是能INT0中断;
//	EINT0 = 1; 		// 待机时再使能
	IPINT0 = 0;

#endif


	#if 0
	// 2个上IFL输入
	P20_Input_Mode; 
	P21_Input_Mode;

	// 配置 INT24/25 上升沿中断
	//下降沿设置
	INT2F = 0X00 ;    //0000 xxxx  0关闭 1使能
	//上升沿设置
	INT2R = 0X30 ;    //0000 xxxx  0关闭 1使能
	
	
	//外部中断优先级设置
	IE1 |= 0x08;	//0000 x000  INT2使能
	IP1 |= 0X00;
	#endif

	
	EA = 1;
}

extern volatile bit bTrigWakeUp;
// 有时会换不醒，不知道什么原因，中间不加判断条件(读电平)也不行
void EX0() interrupt	0
{
//	if (WAKEUP_PIN==0)
		{
		bTrigWakeUp = TRUE;
		// P03 = 0;
		}
}

void EX2(void) interrupt	10
{
   // P26 = ~P26;
	if(P20)
		{
		INT2_flag = 0x10; //INT24产生中断
		bTrigWakeUp = TRUE;
		}

	if(P21)
		{
		INT2_flag = 0x20; //INT25产生中断
		bTrigWakeUp = TRUE;
		}
}

#if 0
/*****************************************************
*函数名称：void EXTI_Test(void)
*函数功能：外部中断测试
*入口参数：void
*出口参数：void
*****************************************************/
void EXTI_Test(void)
{
	EX_Init();
	while(1)
	{					
	}
}
/*****************************************************
*函数名称：void EX_Init(void)
*函数功能：外部中断初始化
*入口参数：void
*出口参数：void
*****************************************************/
void EX_Init(void)
{	
	//配置中断口INT03、INT12/13、INT24/25
	P1CON &= 0X37;     //中断IO口设置为高阻输入
	P1PH  |= 0xC8;     //中断IO口设置为高阻带上拉
	P2CON &= 0XFC;     //中断IO口设置为高阻输入
	P2PH  |= 0x03;     //中断IO口设置为高阻带上拉
 
	//配置INT03上升沿中断、INT12/13下降沿中断、INT24/25双沿中断
    //下降沿设置	
	INT0F = 0X00 ;    //xxxx 0000  0关闭 1使能
	INT1F = 0X0C ;    //xxxx xxxx  0关闭 1使能
    INT2F = 0X30 ;    //0000 xxxx  0关闭 1使能
    //上升沿设置	
	INT0R = 0X08 ;    //xxxx 0000  0关闭 1使能 
	INT1R = 0X00 ;    //xxxx xxxx  0关闭 1使能
	INT2R = 0X30 ;    //0000 xxxx  0关闭 1使能	
	
	//外部中断优先级设置
	IE  |= 0x05;	//0000 0x0x
	IE1 |= 0x08;	//0000 x000  INT2使能
	IP  |= 0X00;
	IP1 |= 0X00;
	EA = 1;
}

/*****************************************************
*函数名称：void EX0/1/2() interrupt	0/2/10
*函数功能：中断服务函数
*入口参数：void
*出口参数：void
*****************************************************/
void EX0() interrupt	0
{
    P26 = ~P26;
}

void EX1() interrupt	2
{
    P27 = ~P27;
	if(P16 == 0)
	{
		INT1_flag = 0x10; //INT12产生中断
	}
	if(P17 == 0)
	{
		INT1_flag = 0x20; //INT13产生中断
	}
}

void EX2() interrupt	10
{
    P26 = ~P26;
}
#endif
