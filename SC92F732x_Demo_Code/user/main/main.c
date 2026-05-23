#include "global.h"

/**************************************************************
说明：
1、Options for Target‘Target1’：BL51 Locate->Code Range:0x100，烧录选项请选择DISRST，复位脚作为普通IO使用；
2、改变TEST的定义，可以分别测试对应的功能；
3、注意：先在Function.H里面选择测试型号（SC92F7320无LCD/LED和PWM功能）
***************************************************************/
#define Test  4    //BTM:0 EXTI:1 Timer:2 LCD:3 PWM:4 Uart0:5 ADC:7 IAP:8 
bit BIT_TMP;

#if 0
// 生成版本号
// build(Feb  4 2023 - 17:49:32) 
//__DATE__="Feb  4 2023"

char code month_names[]="JanFebMarAprMayJunJulAugSepOctNovDec";

void build_FirmVers(void)
{
// RAM: 20+6+1+17 = 44Bytes
//	uint8_t idata Tm_Data[]="Feb  4 2023";
	uint8_t idata FirmVers[20]="V1.00_23.01.01"; // 从Byte 6开始, idx=6
	int idata month, day, year;
	uint8_t idata idx;
	char idata s_month[5];
	char idata s_Data[12];
	
	sprintf(s_Data,__DATE__);
	sscanf(s_Data,"%s %d %d",s_month, &day, &year);

	month = (strstr(month_names,s_month)-month_names)/3+1;
	year = year-2000;

	// 位置要根据软件版本规则, 来修改
	idx = 6;
	FirmVers[idx] = year/10+'0';
	FirmVers[idx+1] = year%10+'0';
	
	FirmVers[idx+3] = month/10+'0';
	FirmVers[idx+4] = month%10+'0';

	FirmVers[idx+6] = day/10+'0';
	FirmVers[idx+7] = day%10+'0';

	printf("Soft_Vers: %s\r\n",FirmVers);
}
#endif

#if 0
void Show_Vers(void)
{
	// 软件版本: 11111
	// 显示版本号: 用3进制显示 R=1, G=2, B=3
	// 11111--33333, 注意要去掉11111/22222/33333这3个数
	#if 0
	// idx=5;
	LED_START_R_ON;
	// idx=4;
	LED_BAT_R_ON;
	// idx=3;
	LED_PST_R_ON;
	// idx=3;
	LED_ERR_R_ON;
	// idx=1;
	LED_SIG_B_ON;
	Delay1ms(1000);
	LED_START_R_OFF;
	LED_BAT_R_OFF;
	LED_PST_R_OFF;
	LED_ERR_R_OFF;
	LED_SIG_B_OFF;
	#endif
	// 版本号显示完成
}
#endif

// init hardware//
#define HARDWAREVERSION							"Rtx415_V3.0"
//#define SOFTWAREVESION						FirmVers
// bit b_IsInitOk=FALSE;

void main(void)
{
	IO_Init();
	InitKeyGpio();
	Init_mI2C();
	
	Timer0_Init();	


	#if 1
	Delay_1ms(500);
	Uart0_Init_T2();	// 9600
	
	printf("\r\n *********%s********** \r\n",HARDWAREVERSION); 
	printf("build("__DATE__ " - " __TIME__ ") \r\n");
	// build_FirmVers();

	Bt_isReady();		// First BT AT command must be AT\r\n\0
	#endif
	
	EX_Init();					// P10 用来做唤醒
	Clr_sys_Para();

	if (Ep24c02_IsReady())
		{
		if (Ep24C02_IsMacOk())
			{
			// 已经配过对了
			}
		}
	
	// 初始值跟着挡位调整；
	
	//WDTCON |= 0x10;		    //清看门狗
	WDG_ReloadCounter;
	
	dPrintf("Init Ok! \n");
	// b_IsInitOk = TRUE;	
	/* Infinite loop */
	while (1)
		{
		WDG_ReloadCounter;
		TimFlg_Hand();

		// test_uart_recv();
		
		WDG_ReloadCounter;
		ScheduleHandle();
		}
}



