#include "global.h"

// 12M 运行, 为什么定时器会慢了1倍, 没有慢, 速度正常的; 
// 24M 只能用于5V, 所以通用速度为12M, 做个宏来解决这个问题

// 时基用Timer1, 串口用Timer2, Timer0预留

void Timer_Init(void);
#if 0
/*****************************************************
*函数名称：void Timer_Test(void);
*函数功能：T0/T1/T2测试
*入口参数：void
*出口参数：void
*****************************************************/
void Timer_Test(void)
{
	Timer_Init();
		while(1)
		{
		WDG_ReloadCounter;
		}
}
#endif

void Timer0_Init(void)
{
	uint16_t u_dly;
	
	// set_Fsys_T0; 	// FT0 = Fsys;
	clr_Fsys_T0;		// FT0 = Fsys/12;
	
	#if 1
	// 1000>256, 所以只能用16bit, 选择mode1
	// T0设置	M1M0=01, 工作方式1, 16bit mode
	set_T0_Mode1;

	u_dly = 65536 - TIMER0_RELOAD_VALUE_1ms;	// TIMER0_RELOAD_VALUE_1ms;
	
	TL0 = LOBYTE(u_dly);    //溢出时间：时钟为Fsys，则12000*（1/Fsys）=1ms;
	TH0 = HIBYTE(u_dly);
	#else
	// 如果是125us的可以用mode2, 执行起来会简单一些
	// T0设置	M1M0=10, Mode=2, 8bit auto reload
	set_T0_Mode2;

	u_dly = 256 - TIMER0_RELOAD_VALUE_125us;
	TL0 = u_dly;
	TH0 = u_dly;
	#endif

	TR0 = 0;
	ET0 = 1;//定时器0允许
	TR0 = 1;//打开定时器0
	
	EA = 1;
}

#if 0

void Timer1_Init(void)
{
	uint8_t u_dly;
	clr_Fsys_T1;	// FT1 = Fsys/12;
	
	//T1设置	M1M0=10, Mode=2, 8bit auto reload
	set_T1_Mode2;

	u_dly = 256 - TIMER1_RELOAD_VALUE_125us;   //溢出时间：时钟为Fsys，则125 *（1/FT1）=125us;
	TL1 = u_dly;
	TH1 = u_dly;
	
	// 用中断判断,前进后退时,会导致433关机不响应
	set_IPT1;	// IPT1 = 1;	//	设置T1的中断优先级为高; davidd 20250315
	
	TR1 = 0;
	ET1 = 1;//定时器1允许
	TR1 = 1;//打开定时器1

	EA = 1;	
}

// 
void Timer2_Init(void)
{
	set_Fsys_T2; 
	
	//T2设置
	
	T2MOD = 0x00;
	T2CON = 0x00;	 				//设置为16位重载寄存器
	RCAP2H = (65536-24000)/256;     //溢出时间：时钟为Fsys，则48000*（1/Fsys）=2ms;
	RCAP2L = (65536-24000)%256;
	TR2 = 0;
	ET2 = 1;//定时器2允许
	TR2 = 1;//打开定时器2
	
	/*
	//Timer2捕获功能
	T2CON = 0X09; //使能EXT2，16位捕获模式
	T2MOD = 0X00;
	TH2 = 0X00;
	TL2 = 0X00;
	RCAP2H = 0X00;
	RCAP2L = 0X00;
	TR2 = 0;
	ET2 = 1;//定时器2允许
	TR2 = 1;//打开定时器2	
	*/
}

#endif

#if 0
/*****************************************************
*函数名称：void Timer_Init(void)
*函数功能：T0/T1/T2初始化
*入口参数：void
*出口参数：void
*****************************************************/
void Timer_Init(void)
{
	TMCON = 0; 
	// TMCON = 0x07;    //------111 ;Timer0、Tiemr1和Tiemr2选择时钟Fsys
	
	Timer0_Init();
//	Timer2_Init();
	
	EA = 1;	
}
#endif

#if 0
/**************************************************
*函数名称：void timer0/1/2() interrupt 1/3/5
*函数功能：定时器中断产生方波
*入口参数：void
*出口参数：void
**************************************************/
void timer0() interrupt 1
{
    TL0 = (65536 - 12000)%256;
	TH0 = (65536 - 12000)/256;
	P02 = ~P02;	
}


void timer1() interrupt 3
{
	P03 = ~P03;
}
#endif

void Timer2Int(void) interrupt 5
{		
	TF2 = 0;   //溢出清零
    P04 = ~P04;
}
