/**********************************************************/
/* ir_433.c file
/* 
/*OBJECT:433M解码程序
/*AUTHOR:David
/*DESCRIPTION:
/*        在100us定时中断中调用433M接收解码函数
/*
/***********************************************************/
/* Private define ------------------------------------------------------------*/
#define __IR_433__

/* -------------------------------Includes --------------------------*/
#include "global.h"
/* -------------------------------variables --------------------------*/
volatile uint8_t    gb_RfRxData0;                //客户代码0
volatile uint8_t    gb_RfRxData1;                //客户代码1
volatile uint8_t    gb_RfRxData2;                //客户代码2

volatile uint8_t    gb_RfRxData3;               //数据码
volatile uint8_t	gb_RfRxStep;                //IR接收步骤
volatile uint16_t    gw_RfRxCnt;                //接收计数器
volatile uint32_t    gew_RfRxData;            	//接收数据暂存，//同步码之后是25位数据，只取24位3个字节

volatile uint8_t    gb_RfRxLevel;
volatile uint8_t    gb_RfRxBitCnt;                //接收数据位
volatile uint8_t 	g_bTimerCount;
volatile uint16_t 	gw_RfRxKeeppingMsCnt;        //接收第一个数据后开始的计数器

volatile bool     	gbit_RfRecOkFlag;            //收到完整的24位数据置1，相当于g_bitKeyDownFlag
volatile bool     	gbit_RfRecSuccess;        	// 接收到有效数据，等待处理

//volatile uint16_t gw_RfRxKeeppingIntervalCount;
// volatile bool     	gbit_RfRecEndCodeFlag;        //收到结束码 
//bool        gbit_ResetFlag;

bit b_IsStudy=FALSE;
bit b_IsStudyOk=FALSE;

uint8_t idata CustCode0=0;
uint8_t idata CustCode1=0;
uint8_t idata CustCode2=0;
uint8_t idata CustSubCode=0;		// 0~3
		
/***************************************************
接收初始化函数
***************************************************/
void RF433M_Init(void)
{
	#if (IC_MODEL == SC92F7351)
    P25_Input_Mode;
	#else
	// P23_Input_Mode
	P23_Quasi_Mode;
	#endif
	
    gb_RfRxStep = 0;
    gbit_RfRecOkFlag = 0;

	g_bTimerCount = 0;
}    

uint8_t IsDataH(void)
{
	if (IR_433M)
		return 0;
	else
		return 1;
}

uint8_t Search_IrCode(uint8_t uIrCode);

/***************************************************
433接收解码函数
在24us定时中断中调用433M接收解码函数, 这里是100us的定时中断里执行
同步码，10ms低电平
之后是25位数据，只取24位3个字节
"1"是1.1ms高+0.2ms低, "0"是0.45高+0.9低
所以可以只通过高电平判断
***************************************************/
volatile uint8_t uDataTmp=0;

void RF433M_RecevieDecode(void)
{
	g_bTimerCount++;
    if(g_bTimerCount == 10) //1ms=42*24us
    	{
		g_bTimerCount = 0;
		gw_RfRxKeeppingMsCnt++;
//		GPIOD->ODR ^= (u8)COLOR_STATE;
    	}

	switch(gb_RfRxStep)
		{
		case 0:
			{
			uDataTmp = IsDataH();
			if(uDataTmp == 0)
				{
				//1-1.开始检测引导码10ms低电平或结束码100ms低电平
				gw_RfRxCnt = 0;
				gb_RfRxStep = 1;
				}  
			}
			break;
		case 1:
			{
			uDataTmp = IsDataH();
			if(uDataTmp == 0)
				{
				//1-2.引导码10ms或结束码100ms低电平计时
				gw_RfRxCnt++;
				}
			else	
				{    
				//1-3.判断引导码9ms低电平或结束码100ms低电平
			#if 0
			// 调试发现，没有发过结束码	; 所以直接去掉这部分代码
				if((gw_RfRxCnt>C_RF_END_L_MIN) && gbit_RfRecOkFlag)
					{
					//结束码
					gbit_RfRecOkFlag = FALSE;
					//相当于按键按下松开时调用函数 -- OnkeyUp()
					// RF433M_OnRecevieEndCode();
					if( gw_RfRxKeeppingMsCnt <= C_DELAY_KEY_PRESS_SHORT)
						{
						gbit_RfRecSuccess = TRUE;
						}
					
					gb_RfRxStep = 0;   
					gew_RfRxData = 0;
					gw_RfRxCnt = 0;
					gb_RfRxBitCnt = 0; 
					return;
					}
				else 
			#endif
				if((gw_RfRxCnt > C_RF_START_L_MAX) || (gw_RfRxCnt < C_RF_START_L_MIN))
					{
					// 不是引导码，所有数据清0
					// 同时用于做为第一个有效数据的判断;
					gb_RfRxStep = 0;   
					gew_RfRxData = 0;
					gw_RfRxCnt = 0;
					gb_RfRxBitCnt = 0; 
					gbit_RfRecOkFlag = FALSE;
					gw_RfRxKeeppingMsCnt = 0;
					// GPIOD->ODR = (u8)(~COLOR_STATE);
					return;          
					}
				//引导码            
				//gbit_RfRecOkFlag = 0;
				//2-1.开始接收数据，检测引导码4.5ms高电平    
				gb_RfRxLevel = uDataTmp;
				gw_RfRxCnt = 0;
				gb_RfRxBitCnt = 0;
				gb_RfRxStep = 2;
			//	gbit_RfRecOkFlag = FALSE;
				gew_RfRxData = 0;
				dPrintf_Ir("s-");
				}
			}
			break;
 
		case 2://check level change
			{
			uDataTmp = IsDataH();
			if (uDataTmp == gb_RfRxLevel)
				{
				if(uDataTmp != 0)
					{
					//电平保持高电平
					gw_RfRxCnt++;
					}
				}
			else
				{    
				//level change,check current level
				gb_RfRxLevel = uDataTmp;
				if (uDataTmp == 0)
					{
					//GPIOD->ODR ^= (u8)COLOR_STATE;	
					//  数据0、1是通过高电平时间判断，所以电平跳变为低的时候，
					//  也就获取到高电平的时间了，
					//  就可以判断有效数据0,1

					// "1"是1.1ms高+0.2ms低, "0"是0.45ms高+0.9ms低
					// 所以可以只通过高电平判断,当然用低电平判断也一样类似
					if((gw_RfRxCnt < C_RF_DATA1_MAX) && (gw_RfRxCnt > C_RF_DATA1_MIN))
						{//data 1
						gb_RfRxBitCnt++; 
						gew_RfRxData = gew_RfRxData << 1;
						gew_RfRxData |= (uint32_t)1;
						dPrintf_Ir("1");
						}    
					else if((gw_RfRxCnt < C_RF_DATA0_MAX) && (gw_RfRxCnt > C_RF_DATA0_MIN))
						{//data 0
						gb_RfRxBitCnt++; 
						gew_RfRxData = gew_RfRxData << 1;
						gew_RfRxData &= (uint32_t)(~1);
						dPrintf_Ir("0");
						}
			#if 1
				// 这个要加上，不然读出的数据容易出错
					else // 数据出错，所有数据清0
						{
						gb_RfRxStep = 0;   
						gew_RfRxData = 0;
						gw_RfRxCnt = 0;
						gb_RfRxBitCnt = 0;
						gbit_RfRecOkFlag = FALSE;
						gw_RfRxKeeppingMsCnt = 0;
					//	GPIOD->ODR ^= (u8)COLOR_STATE;
					//	GPIOD->ODR = (u8)(~COLOR_STATE);
						dPrintf_Ir("-e\r\n");
						return;
						}
			#endif

					if(gb_RfRxBitCnt >= C_RF_REC_BIT_LEN)
						{   
						gew_RfRxData = gew_RfRxData >> 1;
						dPrintf_d("-0x%lx\r\n", gew_RfRxData);
						// 接收完成,设置收到数据标志
						//gb_RfRxData3 = gew_RfRxData $ 0;    //数据码
						//gb_RfRxData2 = gew_RfRxData $ 1;    //客户代码
						//gb_RfRxData1 = gew_RfRxData $ 2;    //客户代码

						#if 1
						// 程序空间没了，用不到的都去掉；
						gb_RfRxData3 = (gew_RfRxData&0x0000000F);   	//数据码
						gb_RfRxData2 = (gew_RfRxData&0x000000F0)>>4;    //客户代码2
						gb_RfRxData1 = (gew_RfRxData&0x000FF00)>>8;   	//客户代码1
						gb_RfRxData0 = (gew_RfRxData&0x00FF0000)>>16;   //客户代码0
						// gbit_RfRecEndCodeFlag = 0;
						#endif
						
						gb_RfRxStep = 0;
						gb_RfRxBitCnt=0;

						if (b_IsStudy)
							{
							// uint8_t idx;
							// 学习模式
							CustCode0 = gb_RfRxData0;
							CustCode1 = gb_RfRxData1;
							CustCode2 = gb_RfRxData2;

							#if 1
							// 正确编码后的处理方式
							if (gb_RfRxData3>0)
								{
								CustSubCode = 0;
								b_IsStudy = FALSE;
								b_IsStudyOk = TRUE;
								}
							#endif
							}
						else
							{
							if ((CustCode0 == gb_RfRxData0)
								&& (CustCode1 == gb_RfRxData1)
								&& (CustCode2 == gb_RfRxData2)
								)
								{
								if(!gbit_RfRecOkFlag)
									{
									gbit_RfRecOkFlag = TRUE;
									//这时候相当于按键的OnkeyDown() 有键按下，
									// RF433M_OnRecevieFirstData();
									gw_RfRxKeeppingMsCnt = 0;
									gbit_RfRecSuccess = TRUE;
									}
								}
							}
				#if 0
						// 这时候相当于按键的OnkeyPressing(); 
						// 长按处理在这里执行，函数里会判断次数；超过2S，认为长按;
						// 低于0.5S的是短按；在放开时处理;RF433M_OnRecevieEndCode()
						// RF433M_OnRecevieData();
						if(gw_RfRxKeeppingMsCnt >= C_DELAY_KEY_PRESS_LONG)
							{
						//	gbit_RfRecSuccess = TRUE;
							}
				#endif
						}
					}
				// 从高电平跳变成低电平，根据gw_RfRxCnt判断完0/1后，清0计数,等待下一个高点
				// 从低跳变成高，此时这里应该一直保持0;
				gw_RfRxCnt = 0;
				}
			}
			break;
		}
}

void Exit_Study(void)
{
	if (b_IsStudyOk)
		{
		b_IsStudyOk = FALSE;
		
		#if 0
		// 从中断中移出来 davidd 20230227
		Save_CustomCode();

		#endif
		}
}

#if 1//433 Data
#define	RF433_KEY_SUM		3		//RF KEY SUM
/*
#define C_RF_DATA_1_KEY     0x01        //A键值		PWR
#define C_RF_DATA_2_KEY     0x02        //B键值		启动
#define C_RF_DATA_3_KEY     0x04        //C键值		停止
#define C_RF_DATA_4_KEY     0x08        //D键值		返回

*/

uint8_t code u_433[3]= {0x01, 0x02, 0x03};

/* Exported variables ------------------------------------------------------- */
#define	IR_EVENT_BASE_ADDR			0xA0

//#define	KEY_EVENT_USER_BASE_ADDR	0xFC
//define key event.
typedef enum
{
	IN_IR_NONE = 0x00,
	IN_IR_PWR = KEY_EVENT_BASE_ADDR,
	
	IN_IR_START,
	IN_IR_BACK,
	IN_IR_STOP
	
} IR433_EVENT;


static IR433_EVENT RF433_KeyEvent[RF433_KEY_SUM] = 
{
	IN_IR_START,
	IN_IR_STOP,
	IN_IR_BACK
};

unsigned char GetRfKeyIndex(void)
{
	unsigned char uKeyIndex=0;

	if (gbit_RfRecSuccess)
		{
		gbit_RfRecSuccess = FALSE;

		uKeyIndex = gb_RfRxData3;
		return uKeyIndex;
		}
	return 0;		// must not uKeyIndex, because is not match, uKeyIndex = RF433_KEY_SUM+1;
}

IR433_EVENT Rf433_KeyEventGet(void)
{
	unsigned char uKeyIndex=0;
	if (uKeyIndex = GetRfKeyIndex())
		return RF433_KeyEvent[uKeyIndex-1];
	else
		return IN_KEY_NONE;
}
//ID_KEY idkey=ID_None;
#define RF_RELCNT   20
// uint8_t uRf_Cnt=0;
void RF433_KeyHandle(void)
{
	IR433_EVENT Rf433_KeyEvent=IN_KEY_NONE;
	Rf433_KeyEvent = Rf433_KeyEventGet();	

	if (Rf433_KeyEvent != IN_IR_NONE)
		{
		#if 0
		// 0.2S内只接收1次遥控键,防止多次触发
		if (TimOut10mS[T433REL]<20)
			return;
		// TimOut10mS[T433REL] = 0;
		#endif	
		
		switch (Rf433_KeyEvent)
			{
			default:
				break;

			
			case IN_IR_STOP:
				
				break;

			case IN_IR_BACK:
				
			break;	
				
			}
		}
}
#endif		// #if 433


