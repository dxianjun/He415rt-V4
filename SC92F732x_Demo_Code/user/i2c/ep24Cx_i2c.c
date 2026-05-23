/**********************************************************/
/* ep24Cx_i2c.c file
/* 
/* Copyright (c) 2002-2005 davidd
/*
/***********************************************************/

#include "global.h"

/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

bool ReadEpData(uint16_t raddr,uint8_t *rdata,uint16_t uNum)
{
	uint16_t page, uTail,uOffset,i;
	bool bRet=FALSE;
	uint8_t *Data = rdata;
	if (uNum == 0)
		return TRUE;
	if ((raddr >= EP24CXX_SIZE) || ((raddr + uNum) > EP24CXX_SIZE))
		return FALSE;

	uOffset = raddr%BYTE_PAGE;
	page = (uNum+uOffset)/BYTE_PAGE;
	if (page > 0)
		uTail = uNum+uOffset-page*BYTE_PAGE;
	else
		uTail = uNum;

	// Page (8 字节)对齐
	if (page == 0)
		{
		return I2C_PolledMasterRead(ADDR_EP24Cxx,(uint8_t)raddr,Data,(uint8_t)uTail);
		}
	
	if (page >= 1)
		{
	//	printf("ReadData Page[1]\n");
		bRet = I2C_PolledMasterRead(ADDR_EP24Cxx,(uint8_t)raddr,Data,(uint8_t)(BYTE_PAGE-uOffset));
		if (bRet == FALSE)
			return bRet;
		Delay_1ms(10);
		
		raddr = raddr+BYTE_PAGE-uOffset;
		Data = Data+BYTE_PAGE-uOffset;
		}
	
	for (i=0;i<page-1;i++)
		{
	//	printf("ReadData Page[%u]\n",i+1);
		bRet = I2C_PolledMasterRead(ADDR_EP24Cxx,(uint8_t)(raddr+i*BYTE_PAGE),Data+i*BYTE_PAGE,BYTE_PAGE);
		if (bRet == FALSE)
			return bRet;
		Delay_1ms(10);
		}
	
	if (uTail>0)
		{
		return I2C_PolledMasterRead(ADDR_EP24Cxx,(uint8_t)(raddr+(page-1)*BYTE_PAGE),Data+(page-1)*BYTE_PAGE,(uint8_t)uTail);
		}
	return TRUE;
}

bool WriteEpData(uint16_t waddr,uint8_t *wdata,uint16_t uNum)
{
	uint16_t page, uTail,i,uOffset;
	bool bRet=FALSE;
	uint8_t *Data = wdata;
	if (uNum == 0)
		return TRUE;
	if ((waddr >= EP24CXX_SIZE) || ((waddr + uNum) > EP24CXX_SIZE))
		return FALSE;

	uOffset = waddr%BYTE_PAGE;
	page = (uNum+uOffset)/BYTE_PAGE;
	if (page > 0)
		uTail = uNum+uOffset-page*BYTE_PAGE;
	else
		uTail = uNum;

	// Page (8 字节)对齐
	if (page == 0)
		{
		return I2C_PolledMasterWrite(ADDR_EP24Cxx,(uint8_t)waddr,Data,(uint8_t)uTail);
		}
	
	if (page >= 1)
		{
		bRet = I2C_PolledMasterWrite(ADDR_EP24Cxx,(uint8_t)waddr,Data,(uint8_t)(BYTE_PAGE-uOffset));
		if (bRet == FALSE)
			return bRet;
		
		waddr = waddr+BYTE_PAGE-uOffset;
		Data = Data+BYTE_PAGE-uOffset;
		Delay_1ms(10);
		}
	
	for (i=0;i<page-1;i++)
		{
		bRet = I2C_PolledMasterWrite(ADDR_EP24Cxx,(uint8_t)(waddr+i*BYTE_PAGE),Data+i*BYTE_PAGE,BYTE_PAGE);
		if (bRet == FALSE)
			return bRet;
		Delay_1ms(10);
		}
	
	if (uTail>0)
		{
		return I2C_PolledMasterWrite(ADDR_EP24Cxx,(uint8_t)(waddr+(page-1)*BYTE_PAGE),Data+(page-1)*BYTE_PAGE,(uint8_t)uTail);
		}

	return TRUE;
}

bool Ep24c02_IsReady(void)
{
	uint8_t Data;
	bool bRet=FALSE;
	
	bRet = I2C_PolledMasterRead(ADDR_EP24Cxx,0,&Data,1);

	return bRet;
}

bool Ep24C02_IsMacOk(void)
{
	uint8_t Data[2];
	bool bRet=FALSE;
	
	bRet = I2C_PolledMasterRead(ADDR_EP24Cxx,0,&Data,2);

	if ((Data[0] == 0xAA) && (Data[1] == 0x55))
		{
		return TRUE;
		}
	else
		{
		return FALSE;
		}
}

#if 0
void ReadExtQuiz(uint8_t * Data)
{
	// 从EPROM中读取 32Byte的数据(sizeof(Out2InTable))
	// 读取时, 一次读取全部答案
	ReadEpData(0,Data,MAX_EROM);
}

void SaveExtQuiz(uint8_t AnswerIdx, uint8_t * Data)
{
	// 从EPROM中读取 8Byte的数据(sizeof(Out2InTable[AnswerIdx]))
	WriteEpData(8*AnswerIdx,Data,8);
}

void ClearExtQuiz(void)
{
	uint8_t Data[MAX_EROM]={0};
	WriteEpData(0,Data,MAX_EROM);
}
#endif

#if 0
void TestEp24c512(void)
{
	uint8_t WriteBuffer[128];
	uint8_t ReadBuffer[128];
	uint16_t Addr;
	uint16_t page;
	
	for(Addr=0; Addr<128; Addr++)
	WriteBuffer[Addr]=Addr;	 /* 填充WriteBuffer */

	for (page=0;page<512;page++)
		{
		/* 开始向EEPROM写数据 */
	 	dPrintf("\r\n EEPROM 24C512 Write Test \r\n",0);
	  	I2C_PagerWrite(0xA0,128*page,WriteBuffer,sizeof(WriteBuffer));
	 	dPrintf("EEPROM 24C512 Write Page[%u] OK \r\n",page);

		/* 开始读取 EEPROM 数据 */
	 	dPrintf("EEPROM 24C512 Read Test \r\n",0);
	  	I2C_PageRead(0xA0,128*page,ReadBuffer,sizeof(ReadBuffer));

		for (Addr=0; Addr<128; Addr++)
			{
			if (WriteBuffer[Addr] != ReadBuffer[Addr])
				{
				printf("24C512 Page[%u][%u] Error \r\n",page,Addr);
				}
			}
	 	dPrintf("\r\n EEPROM 24C512 Read Page[%u] OK \r\n",page);

		Delay_ms(100);
		}
}

void TestEp24c32(void)
{
	uint8_t WriteBuffer[BYTE_PAGE];
	uint8_t ReadBuffer[BYTE_PAGE];
	uint16_t Addr;
	uint16_t page;
	uint16_t pageTotal;

	pageTotal = 4096/BYTE_PAGE;
	
	for(Addr=0; Addr<BYTE_PAGE; Addr++)
		WriteBuffer[Addr]=Addr;	 /* 填充WriteBuffer */

	for (page=0;page<pageTotal;page++)
		{
		/* 开始向EEPROM写数据 */
	 	dPrintf("\r\nEEPROM 24C32 Write Test \r\n",0);
		WriteEpData(BYTE_PAGE*page,WriteBuffer,sizeof(WriteBuffer));
	  	//I2C_PagerWrite(0xA0,BYTE_PAGE*page,WriteBuffer,);
	 	dPrintf("EEPROM 24C32 Write Page[%u] OK \r\n",page);

		/* 开始读取 EEPROM 数据 */
	 	dPrintf("EEPROM 24C32 Read Test \r\n",0);
	  	//I2C_PageRead(0xA0,BYTE_PAGE*page,ReadBuffer,sizeof(ReadBuffer));
	  	memset(ReadBuffer,0,sizeof(ReadBuffer));
		ReadEpData(BYTE_PAGE*page,ReadBuffer,sizeof(ReadBuffer));
		for (Addr=0; Addr<BYTE_PAGE; Addr++)
			{
			printf("Read Data[%u][%u] = [%u] \r\n",page,Addr,ReadBuffer[Addr]);
			if (WriteBuffer[Addr] != ReadBuffer[Addr])
				{
				printf("24C32 Page[%u][%u] Error \r\n",page,Addr);
				}
			}
	 	dPrintf("\r\nEEPROM 24C32 Read Page[%u] OK \r\n",page);

		Delay_ms(100);
		}
}
#endif





