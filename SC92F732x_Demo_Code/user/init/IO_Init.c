#include "global.h"
/*****************************************************
*函数名称：void IO_Init(void)
*函数功能：IO初始化
*入口参数：void
*出口参数：void
*****************************************************/

/*
#define CTRL_BT_RST			P16		// PPH，长低会让蓝牙复位
#define CTRL_LED_R			P17		// PPH 红灯，待机时亮，倾斜检测到位后，开机前
#define CTRL_LED_K4			P27		// PPH 低电平亮，K4指示灯，大水量
#define CTRL_LED_K3			P26		// P2.6 双灯复用：0亮小水量灯，1亮停止灯，输入高阻两灯灭

*/
void IO_Init(void)
{
	P16_PushPull_Mode; BT_RST_REL;
	P17_PushPull_Mode; LED_SLEEP_OFF;		
	P27_PushPull_Mode; LED_MODE3_OFF;
	LED_WATER_BOTH_OFF;
	
	// 在pwm处单独设置
//	P00_PushPull_Mode; PWM0_HIGH;		// 转盘的PWM低电平有效
//	P01_PushPull_Mode; PWM1_HIGH;
	// 输入上拉
	P10_Quasi_Mode; P10=1;	// 上拉输入，这颗芯片不用配置smit
	P25_Quasi_Mode; P25=1;	// K3 小水量按键
	// P10_Input_Mode;				// 外部下拉，高电平为有效电平

#if 0	// 都用F7321/22
	#if (IC_MODEL == SC92F7351)
	P25_PushPull_Mode; CTRL_LED3=LED_OFF;
	#elif (IC_MODEL == SC92F7321)
	P23_PushPull_Mode; CTRL_LED3=LED_OFF;
	#endif
#endif

	#if (IC_MODEL == SC92F7351)
	SC92F7351_NIO_Init(); //未引出IO口配置
	#endif
	
	#if (IC_MODEL == SC92F7321)
	SC92F7321_NIO_Init(); //未引出IO口配置
	#endif
}
	
#if 0
// SC92F735x
void IO_Init(void)
{
	#if (IC_MODEL == SC92F7352)  //SC92F7352引脚定义
	P0CON = 0xFF;  //设置P0为强推挽模式
	P0PH  = 0x00;
	P1CON = 0x00;  //设置P1为高阻输入模式
	P1PH  = 0x00;
	P2CON = 0x00;  //设置P2为高阻带上拉模式
	P2PH  = 0xFF;
	#endif
	
	#if (IC_MODEL == SC92F7351)  //SC92F7352引脚定义
	P0CON = 0xFF;  //设置P0为强推挽模式
	P0PH  = 0x00;
	P1CON = 0x00;  //设置P1为高阻输入模式
	P1PH  = 0x00;
	P2CON = 0x00;  //设置P2为高阻带上拉模式
	P2PH  = 0xFF;
	SC92F7351_NIO_Init(); //未引出IO口配置
	#endif
	
	#if (IC_MODEL == SC92F7350)  //SC92F7350引脚定义
	P0CON = 0xFF;  //设置P0为强推挽模式
	P0PH  = 0x00;
	P1CON = 0x00;  //设置P1为高阻输入模式
	P1PH  = 0x00;
	P2CON = 0x00;  //设置P2为高阻带上拉模式
	P2PH  = 0xFF;
	SC92F7350_NIO_Init(); //未引出IO口配置
	#endif
	
}
#endif


#if 0
// SC92F732x
void IO_Init(void)
{
	#if (IC_MODEL == SC92F7323)  //SC92F7323引脚定义
	P0CON = 0x00;  //设置P0为高阻带上拉模式
	P0PH  = 0xFF;
	P1CON = 0x00;  //设置P1为高阻输入模式
	P1PH  = 0x00;
	P2CON = 0xFF;  //设置P2为强推挽模式
	P2PH  = 0x00;
	P5CON = 0xFF;  //设置P5为强推挽模式
	P5PH  = 0x00;
	#endif
	
	#if (IC_MODEL == SC92F7322)  //SC92F7323引脚定义
	P0CON = 0x00;  //设置P0为高阻带上拉模式
	P0PH  = 0xFF;
	P1CON = 0x00;  //设置P1为高阻输入模式
	P1PH  = 0x00;
	P2CON = 0xFF;  //设置P2为强推挽模式
	P2PH  = 0x00;
	SC92F7322_NIO_Init(); //未引出IO口配置
	#endif
	
	#if (IC_MODEL == SC92F7321)  //SC92F7321引脚定义
	P0CON = 0x00;  //设置P0为高阻带上拉模式
	P0PH  = 0xFF;
	P1CON = 0x00;  //设置P1为高阻输入模式
	P1PH  = 0x00;
	P2CON = 0xFF;  //设置P2为强推挽模式
	P2PH  = 0x00;
	SC92F7321_NIO_Init(); //未引出IO口配置
	#endif
	
	#if (IC_MODEL == SC92F7320)  //SC92F7320引脚定义
	P1CON = 0x00;  //设置P1为高阻输入模式
	P1PH  = 0x00;
	P2CON = 0xFF;  //设置P2为强推挽模式
	P2PH  = 0x00;
	SC92F7320_NIO_Init(); //未引出IO口配置
	#endif
}
#endif


