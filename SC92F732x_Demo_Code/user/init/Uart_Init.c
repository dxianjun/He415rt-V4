#include "global.h"

void Uart0_Init_T1(uint32_t u32Baudrate);    //选择Timer1作为波特率信号发生器
// void Uart0_Init_T2(uint32_t u32Baudrate);    //选择Timer2作为波特率信号发生器
void Uart0_Init_T2(void);

bit UartSendFlag = 0; 		//发送中断标志位



#if USE_UART0_ISR
volatile unsigned char idata TCP_Data_count0=0;
volatile unsigned char xdata TCP_ISR_buf0[MAX_BUF_SIZE];
#endif

#if 0
/*****************************************************
*函数名称：void Uart0_Test(void)
*函数功能：Uart0测试
*入口参数：void
*出口参数：void
*****************************************************/
void Uart0_Test(void)
{
	Uart0_Init_T2();	// 9600
	while(1)
	{
		SBUF = 0xAA;
		while(!UartSendFlag);
		UartSendFlag = 0;
	}
}
#endif

#if 0
/*****************************************************
*函数名称：void Uart0_Init(void)
*函数功能：Uart0中断初始化
*入口参数：void
*出口参数：void
*****************************************************/
void Uart0_Init_T1(uint32_t u32Baudrate)    //选择Timer1作为波特率信号发生器
{
	uint8_t u_BaudRate_dly;
	
	P1CON &= 0xF3;	 // P12, P13配置为输入, // P12 Rx, P13 Tx
	P1PH |= 0x0C;	 	//TX/RX为带上拉输入；	
	// P13 = 1;		 //TX初始高电平；
	
	set_Uart_Mode1;  //方式1，允许接收数据
	set_REN;
	/*
	SCON = 0x50;    
	M0M1=01, mode1
	REN=1
	
	串行通信模式控制位, 常用的通信方式为模式1
	00：模式 0，8 位半双工同步通信模式，在 RX 引脚上收发串行数据。TX 引脚用作发送移位时钟。每帧收发 8 位，低位先接收或发送；
	01：模式 1，10 位全双工异步通信，由 1 个起始位，8 个数据位和 1 个停止位组成，通信波特率可变；
	10：模式 2，11 位全双工异步通信，由 1 个起始位，8 个数据位，一个可编程的第 9 位和 1 个停止位组成；
	11：模式 3，11 位全双工异步通信，由 1 个起始位，8 个数据位，一个可编程的第 9 位和 1 个停止位组成，通信波特率可变。
	*/
	
	set_baud_from_T1; 	// 使用定时器1作UART时钟
	
	set_Fsys_T1;		// TMCON = 0x02;    //定时器1   Fsys；
	set_T1_Mode2;		// TMOD = 0x20;     //定时器1  8位自动重载
	
	set_SMOD;		// PCON |= 0x80; 	// 
	// fn1=Fsys/12 or fn=Fsys
	// BaudRate = 2^smod/16*fn1/dly/2; --> BaudRate = 2^smod/32*fn1/(256-TH1);  2/32*12M/dly ; 
	// dly = 12M/16/baudrate = 12*62500/baudrate;

	// FSYS=12M是, 62500*__FSYS__ = 750000, 刚好是T2的2倍
	u_BaudRate_dly = (uint8_t)((double)(62500.0)*__FSYS__/u32Baudrate+0.5);	// 39 //UART 波特率24M情况下=38400； 12M -->19200
	
	u_BaudRate_dly = 256-u_BaudRate_dly;		// 217  
	
	TL1 = TH1 = u_BaudRate_dly;				
	
	TR1 = 1;		 	//启动Timer0

	EUART = 1;	    	//允许UART中断
//	EA = 1;		     	//开总中断
}
#endif
// T1 被Ir_433占用, 这里串口用T2

#if 0
void Uart0_Init_T2(void)    //选择Timer2作为波特率信号发生器
{
	P1CON &= 0XF3;
	P1PH |= 0X0C;	 		//TX为强推输出，RX为带上拉输入；
	
	SCON  = 0X50;    //设置通信方式为模式一，允许接收
	PCON |= 0X80; 
	TMCON |= 0X04;
	T2MOD = 0X00;
	T2CON = 0X30;
	
	TL2=RCAP2L = 0xD9;		// B1;		// 9600 = -39(FFD9)  4800=-79(FFB1)
	TH2=RCAP2H = 0xFF;
	
	TR2 = 1;
	EUART = 1;     //开启Uart0中断
	EA = 1;
}	
#else
void Uart0_Init_T2(void)    //选择Timer2作为波特率信号发生器
{
	// uint16_t u_BaudRate_dly;
	// 设置IO配置
	P1CON &= 0xF3;	 // P12, P13配置为输入, // P12 Rx, P13 Tx
	P1PH |= 0x0C;	 // TX/RX为带上拉输入；	
	P13=1;
	
	//设置通信方式为模式1，并且允许接收
	set_Uart_Mode1;		// M0M1=01, mode1
	set_REN;	
	/*
	SCON = 0x50;  -- >  M0M1=01, mode1 & REN=1
	
	串行通信模式控制位, 常用的通信方式为模式1
	00：模式 0，8 位半双工同步通信模式，在 RX 引脚上收发串行数据。TX 引脚用作发送移位时钟。每帧收发 8 位，低位先接收或发送；
	01：模式 1，10 位全双工异步通信，由 1 个起始位，8 个数据位和 1 个停止位组成，通信波特率可变；
	10：模式 2，11 位全双工异步通信，由 1 个起始位，8 个数据位，一个可编程的第 9 位和 1 个停止位组成；
	11：模式 3，11 位全双工异步通信，由 1 个起始位，8 个数据位，一个可编程的第 9 位和 1 个停止位组成，通信波特率可变。
	*/
	clr_SMOD;			// PCON |= 0x80; 在方式1和方式3时,波特率来自T1或者T2的溢出率, 看对应的计算公式; 用T1时有效, T2时无效

	T2MOD = 0x00;
	T2CON = 0x00;	 		//设置为16位重载寄存器
	
	set_baud_from_T2; 		// T2CON = 0x30;
	set_Fsys_T2;					// TMCON |= 0x04;	
	
	//T2设置
	// fn2=Fsys/12 or fn2=Fsys
	// BaudRate = 1/16*fn2/dly/2=FSYS/32/dly; --> BaudRate = FSYS/32/dly; 
	// dly = 12M/32/baudrate = 12M/32/baudrate;

	// FSYS=12M时, 31250*12=375000
	// 39.06 -- 9600,  有偏差
	// u_BaudRate_dly = (uint16_t)((double)(31250.0)*__FSYS__/u32Baudrate+0.5);	//如果dly=39, UART 波特率24M情况下=19200； 12M -->9600
	// u_BaudRate_dly = -u_BaudRate_dly;	

	TL2=RCAP2L = 0xD9; 		// 0xB1; 0xD9;
	TH2=RCAP2H = 0xFF;	// HIBYTE(u_BaudRate_dly); 		// 0xFF;	

	TR2 = 1;
	
#if 1// USE_UART0_ISR
	set_EUART;     //开启Uart0中断
	// memset(TCP_ISR_buf0,0,MAX_BUF_SIZE);
#endif

	EA = 1;
}
#endif

#define TDLY 0

/*****************************************************
*函数名称：void UartInt(void) interrupt 4
*函数功能：Uart0中断服务函数
*入口参数：void
*出口参数：void
*****************************************************/
#if 0
bit UartReceiveFlag = 0; 	//接收中断标志位
uchar idata u_data=0x55;

void Uart_Isr(void) interrupt 4
{
	if(TI)
	{
		TI = 0;	
		UartSendFlag = 1;		
	}
	if(RI)
	{
		u_data = SBUF;
		RI = 0;	
		UartReceiveFlag = 1;
	}	
}

void test_uart_recv(void)
{
	if (UartReceiveFlag)
		{
		Send_Data_To_UART0(u_data);
		}
}

#endif
#if USE_UART0_ISR
void Uart_Isr(void) interrupt 4 				 //interrupt address is 0x0023
{
	if(TI)
		{
		clr_TI;                             /* if emission occur */
		UartSendFlag = 1;
		}
	
	if (RI) 
		{                                       /* if reception occur */
		clr_RI;                             /* clear reception flag for next reception */

		if(TCP_Data_count0<(MAX_BUF_SIZE-1))
			{
			TCP_ISR_buf0[TCP_Data_count0] = SBUF;			
			TCP_Data_count0++;
			}
		else 	
			TCP_Data_count0 = 0;
		
		TimOut1mS[TUART0] = 0;				// 数据包每帧延时10mS;
		}
}
#endif

#if 0
uint8_t Receive_Data_From_UART0(void)
{
    uint8_t idata c;
    while (!RI);
    c = SBUF;
    RI = 0;
    return (c);
}
#endif

#if 1

void Send_Data_To_UART0 (uint8_t c)
{
	TimOut1mS[TDLY]=0;
	UartSendFlag =0;

    TI = 0;
    SBUF = c;
    while(TI==0)
		{
		if (UartSendFlag)
			{
			UartSendFlag =0;
			break;
			}

		WDG_ReloadCounter;
		TimFlg_Hand();
		if (TimOut1mS[TDLY]>=10)
			break;
		}
}

char putchar (char c)
{
	#if __DBG_EN__
  /* Write a character to the UART0 */
	Send_Data_To_UART0(c);
	#endif
	
	return (c);
}

void UART0_Send_string( char *str)						// debug uart;
{
	while((*str)!='\0') 								//USART1_SendByte(*str++);
	{
		Send_Data_To_UART0(((u16)(*str++ )& (u16)0x01FF));
	}
}

void UART0_Send_Buf(unsigned char *Buf, unsigned char len)
{
	while (len--)	// 数据可能是0x00('\0')
	{
		Send_Data_To_UART0(((u16)(*Buf++ )& (u16)0x01FF));
	}
}


#endif


