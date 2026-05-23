/**********************************************************/
/* Uart.c file
/* 
/* Copyright (c) 2013 davidd
/*
/***********************************************************/
/* Private define ------------------------------------------------------------*/
#define __UART__

/* -------------------------------Includes --------------------------*/
#include "global.h"

/* -------------------------------variables --------------------------*/
//below is for UART;

uint8_t	xdata TCP_REC_buf[MAX_BUF_SIZE];
uint8_t xdata TCP_SEND_buf[MAX_BUF_SIZE];
UartData_TypeDef xdata TCP_BUF_PION;

/* -------------------------------Function --------------------------*/

uint8_t *mydatacpy(uint8_t* dataDest, const uint8_t* dataScr, uint16_t len)	// 数据复制函数
{
	uint8_t* temp=dataDest;
	uint8_t i=0;
	for (i=0;i<len;i++)
		{
			*temp++ = *dataScr++;
		}
	return dataDest;
}

unsigned char checkSum(unsigned char *buf, unsigned char len)
{
	unsigned char i,tempq=0;
	unsigned char *uTmp=buf;

	for(i=0;i<len;i++)
	{
		tempq += *uTmp;
		uTmp++;
	}
//	tempq=(~tempq)+1;
	return(tempq);
}

void PrintData2(uint8_t* databuf,uint8_t len)
{
	#if (__DBG_EN__== 1)
		{
		uint8_t data tmp;	
		for(tmp = 0;tmp < len;tmp++)
			{
			printf("%02x ",(uint16_t)databuf[tmp]);
			}
		printf("\r\n");
		}
	#endif
}

void ParseUartCmd(UartData_TypeDef* UartData);
void ConsoleUartCmd_App(uint8_t idx);
uint8_t idata uc_LastCmd=0;
uint8_t idata bBtReady=FALSE;
uint8_t idata bBtReadMacPending=FALSE;
uint8_t idata bBtReadMacWaiting=FALSE;
void Resend_Info(void);
uint8_t TrySaveMacResponse(uint8_t *pbuf);
uint8_t TryParseBtReady(uint8_t *pbuf);

void Send_ReadMacReqFrame(void);
void Bt_PairingTimeoutHandle(void);

void Tcp_Hand(void)
{
	if (TCP_Data_count0 > 3)
		{
		if (TimOut1mS[TUART0] >= 5)	// 5ms内没有数据传输,表示数据传输结束, 相当于arm中UART_IT_RTIS
			{
			TimOut1mS[TUART0] = 0;

			// 只有1个串口，不需要那么复杂
			mydatacpy(TCP_REC_buf,TCP_ISR_buf0,TCP_Data_count0);
			TCP_BUF_PION.UartDataType = DATA_FROM_APP;
			TCP_BUF_PION.UartData_Len = TCP_Data_count0;
			TCP_REC_buf[TCP_Data_count0] = 0;
		  	TCP_Data_count0 = 0;
			memset(TCP_ISR_buf0,0,MAX_BUF_SIZE);

			ParseUartCmd(&TCP_BUF_PION);
				
			memset(TCP_REC_buf,0,MAX_BUF_SIZE);
			memset(&TCP_BUF_PION,0,sizeof(UartData_TypeDef));
			}
		}
#if 1
	// 超时重新发送
	if (TimOut10mS[TACK_TMO] >= TIMOUT_INFO_10MS)
		{
		TimOut10mS[TACK_TMO] = 0;	// every 1s 1 time

		if (uc_LastCmd != 0)
			{
			//超时重新发送 
			Resend_Info();
			}
		}
#endif
	Bt_PairingTimeoutHandle();
}

#define HEAD_VALUE_RECV		0xA6
#define HEAD_VALUE_SEND		0xB6
#define TAIL_VALUE_RECV		0xFF
#define BT_MAC_RSP_TIMEOUT_10MS 200

#define DBG_ASCII					0
void ParseUartCmd(UartData_TypeDef* UartData)
{
	if (DATA_FROM_APP == UartData->UartDataType)
		{
		uint16_t idata i, len, packlen;
		len = UartData->UartData_Len;

	#if 0
		#if DBG_ASCII
		PrintData2(TCP_REC_buf, len);
		#else
		UART0_Send_Buf(TCP_REC_buf,len);
		#endif
	#endif
	
		// 带网络参数的统一指令
		for (i=0;i<len;i++)
			{
			/*
			主板下发	包头		cmd len data	结束
					A5		cmd xxx datax	0xFF
			*/
			if (TCP_REC_buf[i]== HEAD_VALUE_RECV)
				{
				packlen = TCP_REC_buf[i+2];	// plen=1时，tail=4; plen=0时，tail=3
				if (TCP_REC_buf[i+packlen+3]== TAIL_VALUE_RECV)
					{
					ConsoleUartCmd_App(i);
					}
				}
			else
				{
				#if 0
				// 透过串口直接配置信息
				if ((TCP_REC_buf[i] == 'A') &&(TCP_REC_buf[i+1] == 'T'))
					{
					// 转发给BT模块
					UART0_Send_string(&TCP_REC_buf[i]);
					}
				else 
				#endif
				if (TCP_REC_buf[i]== '+')		// 回复的信息都是以+开头，以\r\n\0结尾
					{
					TryParseBtReady(&TCP_REC_buf[i]);
					TrySaveMacResponse(&TCP_REC_buf[i]);
					printf("%s", &TCP_REC_buf[i]);
					}
				}
			}
		}
}

/*
按键板上传	包头		cmd len data	结束
		B6		cmd xxx datax	0xFF
*/
#define CMD_SEND_ACK	0x10

#define CMD_KEY_PWR		0x01
#define CMD_KEY_WATER	0x02
#define CMD_KEY_MOT		0x03		// 0: 停止 1: 左持续  	 2：右持续 3：左180			4：右180

/*
主板下发		cmd	len	data	
应答	01	0	3	
*/
#define CMD_RECV_ACK	0x10
#define CMD_RECV_INFO	0x11
#define CMD_READ_MAC	0x20

const uint8_t cmd_bt_ready[] = "AT\r\n\0";

uint8_t TryParseBtReady(uint8_t *pbuf)
{
	if ((pbuf[0] != '+') || ((pbuf[1] != 'o') && (pbuf[1] != 'O')) || ((pbuf[2] != 'k') && (pbuf[2] != 'K')))
		{
		return 0;
		}

	bBtReady = TRUE;
	if (bBtReadMacPending)
		{
		bBtReadMacPending = FALSE;
		Send_ReadMacReqFrame();
		}
	return 1;
}

void Bt_isReady(void)
{
	bBtReady = FALSE;
	UART0_Send_string((char *)cmd_bt_ready);
}

uint8_t Bt_GetReady(void)
{
	return bBtReady;
}
#define MAC_RESP_PREFIX_LEN 5
#define MAC_ASCII_LEN       12
#define EP_MAC_SAVE_ADDR    0x08

uint8_t IsMacHexChar(uint8_t ch)
{
	if ((ch >= '0') && (ch <= '9'))
		{
		return 1;
		}
	if ((ch >= 'A') && (ch <= 'F'))
		{
		return 1;
		}
	if ((ch >= 'a') && (ch <= 'f'))
		{
		return 1;
		}
	return 0;
}


uint8_t cmd_bt_pair[] = "AT+FTM=19667F4A76FC\r\n\0";


uint8_t TrySaveMacResponse(uint8_t *pbuf)
{
	uint8_t idata i;

	if ((pbuf[0] != '+') || (pbuf[1] != 'M') || (pbuf[2] != 'A') || (pbuf[3] != 'C') || (pbuf[4] != ':'))
		{
		return 0;
		}

	for (i=0;i<MAC_ASCII_LEN;i++)
		{
		if (0 == IsMacHexChar(pbuf[MAC_RESP_PREFIX_LEN+i]))
			{
			return 0;
			}
		}

	if (FALSE == bBtReadMacWaiting)
		{
		return 0;
		}

	// 读出MAC地址后，执行配对
	for (i=0; i<MAC_ASCII_LEN; i++)
		{
		cmd_bt_pair[7+i] = pbuf[MAC_RESP_PREFIX_LEN+i];
		}
	UART0_Send_string((char *)cmd_bt_pair);

	// MAC地址写入Eprom, 并且打标记
	if (WriteEpData(EP_MAC_SAVE_ADDR, &pbuf[MAC_RESP_PREFIX_LEN], MAC_ASCII_LEN))
		{
		uint8_t Data[]={0xAA, 0x55};
		
		bBtReadMacWaiting = FALSE;
		TimOut10mS[TPAIR_RSP] = 0;

		// 打标记
		WriteEpData(0, Data, 2);	
		return 1;
		}

	return 0;
}


void Bt_PairingTimeoutHandle(void)
{
	if ((bBtReadMacWaiting) && (TimOut10mS[TPAIR_RSP] >= BT_MAC_RSP_TIMEOUT_10MS))
		{
		bBtReadMacWaiting = FALSE;
		bBtReadMacPending = FALSE;
		TimOut10mS[TPAIR_RSP] = 0;
		}
}

void Send_ReadMacReqFrame(void)
{
	TCP_SEND_buf[0]=HEAD_VALUE_SEND;
	TCP_SEND_buf[1]=CMD_READ_MAC;
	TCP_SEND_buf[2]=0;
	TCP_SEND_buf[3]=0xFF;

	UART0_Send_Buf(TCP_SEND_buf,4);

	bBtReadMacWaiting = TRUE;
	TimOut10mS[TPAIR_RSP] = 0;
}

void Pairing_StartReadMac(void)
{
	if (FALSE == bBtReady)
		{
		bBtReadMacPending = TRUE;
		Bt_isReady();
		return;
		}

	bBtReadMacPending = FALSE;
	Send_ReadMacReqFrame();
}

#define IDX_CMD		1
#define IDX_LEN		2
#define IDX_DAT0	3


// 上传
// 包头 	cmd len datax 结束
// 0xB6  10 xx	datax	FF

void Send_BackAck(uint8_t uc_ack, uint8_t uc_cmd, uint8_t uc_para)
{
	TCP_SEND_buf[0]=HEAD_VALUE_SEND;
	TCP_SEND_buf[IDX_CMD]=CMD_SEND_ACK;
	TCP_SEND_buf[IDX_LEN]=1;	// ack长度为1
	TCP_SEND_buf[IDX_DAT0]=uc_ack;
	TCP_SEND_buf[IDX_DAT0+1]=uc_cmd;
	TCP_SEND_buf[IDX_DAT0+2]=uc_para;
	
	TCP_SEND_buf[6]=0xFF;

	UART0_Send_Buf(TCP_SEND_buf,uc_para);
}

void Send_Info_power(uint8_t u_power)
{
	TCP_SEND_buf[0]=HEAD_VALUE_SEND;
	TCP_SEND_buf[IDX_CMD]=CMD_KEY_PWR;
	TCP_SEND_buf[IDX_LEN]=1;	// ack长度为1
	TCP_SEND_buf[IDX_DAT0]=u_power;
	TCP_SEND_buf[4]=0xFF;

	UART0_Send_Buf(TCP_SEND_buf,5);

	TimOut10mS[TACK_TMO] = 0;	// every 1s 1 time
	uc_LastCmd = CMD_KEY_PWR; 
}

// 0~3
void Send_Info_waterSet(uint8_t uc_waterSet)
{
	if (uc_work_mode > MODE_SLEEP)
		{
		TCP_SEND_buf[0]=HEAD_VALUE_SEND;
		TCP_SEND_buf[IDX_CMD]=CMD_KEY_WATER;
		TCP_SEND_buf[IDX_LEN]=1;	
		TCP_SEND_buf[IDX_DAT0]=uc_waterSet;
		TCP_SEND_buf[4]=0xFF;

		UART0_Send_Buf(TCP_SEND_buf,5);

		TimOut10mS[TACK_TMO] = 0;	// every 1s 1 time
		uc_LastCmd = CMD_KEY_WATER; 
		}
}

// 0~4
void Send_Info_MotSet(uint8_t uc_motSet)
{
	if (uc_work_mode > MODE_SLEEP)
		{
		TCP_SEND_buf[0]=HEAD_VALUE_SEND;
		TCP_SEND_buf[IDX_CMD]=CMD_KEY_MOT;
		TCP_SEND_buf[IDX_LEN]=1;	
		TCP_SEND_buf[IDX_DAT0]=uc_motSet;
		TCP_SEND_buf[4]=0xFF;

		UART0_Send_Buf(TCP_SEND_buf,5);

		TimOut10mS[TACK_TMO] = 0;	// every 1s 1 time
		uc_LastCmd = CMD_KEY_MOT; 
		}
}

void Resend_Info(void)
{
	switch (uc_LastCmd)
		{
		default: break;

		case CMD_KEY_PWR:
			{
			UART0_Send_Buf(TCP_SEND_buf,5);

			uc_LastCmd = 0;
			}
			break;

		case CMD_KEY_WATER:
			{
			UART0_Send_Buf(TCP_SEND_buf,5);

			uc_LastCmd = 0;
			}
			break;

		case CMD_KEY_MOT:
			{
			UART0_Send_Buf(TCP_SEND_buf,5);

			uc_LastCmd = 0;
			}
			break;
		}
}

// 
// B6 03 01 01 FF  调试让主板开机；

#define SET_PWR_OFF			0
#define SET_PWR_PRE_OFF		1
#define SET_PWR_ON			2

#define WATER_SPRING_STOP			0
#define WATER_SPRING_ONCE			1
#define WATER_SPRING_CONT			2

// 包头 	cmd len datax 结束
// 0xA5  11 xx	datax	FF

uint8_t idata uc_dutybk=0;
uint8_t idata uc_pwr_state=0;			// 0: 关机; 1: 预关机; 2:开机

void ConsoleUartCmd_App(uint8_t idx)
{
	uint8_t idata uc_cmd=0, uc_tmp=0;

	uc_cmd = TCP_REC_buf[idx+IDX_CMD];
	switch (uc_cmd)
		{
		default: break;

		#if 0
		case CMD_READ_MAC:
			{
			// Send read MAC request frame: B6 20 00 FF
			Pairing_StartReadMac();
			}
			break;
		#endif
		case CMD_RECV_INFO:
			{
			#if DBG_ASCII
				printf("power %d\r\n", (uint16_t)uc_tmp);
			#endif
			uint8_t uc_tmp_cmd = TCP_REC_buf[idx+IDX_DAT0];
			uc_tmp = TCP_REC_buf[idx+IDX_DAT0+1];
			
			// 功能暂时不确定，先不管
			Send_BackAck(0, uc_tmp_cmd, uc_tmp);	
			}
			break;

		
		case CMD_RECV_ACK:
			{
			uc_tmp = TCP_REC_buf[idx+IDX_DAT0];

			#if DBG_ASCII
				printf("duty %d\r\n", (uint16_t)uc_tmp);
			#endif

			// 收到ack，不管命令对不对，都当是处理
			uc_LastCmd = 0;
			}
			break;
		}
}



















