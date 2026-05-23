/**********************************************************/
/* uart.h file
/* 
/* Copyright (c) 2013 davidd
/*
/***********************************************************/
#ifndef __UART_H__
#define __UART_H__
/* Includes ----------------------------------------------------------------*/

/* public defines -----------------------------------------------------------*/


/* Exported variables ------------------------------------------------------- */

#if 0
typedef struct _UartData
{
	unsigned char m_flag;			// 标准精度和高精度，预留,同时用来占位
	unsigned char m_rsv;
	
	unsigned short m_MaxPower;		// 1W
	unsigned short m_MinPower;		// 1W
	unsigned short m_MaxVolt;		// 0.1V
	
} UartData;

extern UartData gUartData;
#endif
/* Exported functions ------------------------------------------------------- */
void Init_Uart(u32 uBaudRate);

void Tcp_Hand(void);
void Send_Info_waterFull(void);
void PrintData2(uint8_t* databuf,uint8_t len);
void UART0_Send_string( char *str);
void UART0_Send_Buf(unsigned char *Buf, unsigned char len);
void test_uart_recv(void);

void Send_Info_MotSet(uint8_t uc_motSet);
void Send_Info_waterSet(uint8_t uc_waterSet);
void Send_Info_power(uint8_t u_power);
void Pairing_StartReadMac(void);
void Bt_isReady(void);
uint8_t Bt_GetReady(void);


#endif



