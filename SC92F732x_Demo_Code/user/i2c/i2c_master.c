/**********************************************************/
/* i2c_master.c file
/* 
/* Copyright (c) 2002-2005 davidd
/*
/***********************************************************/
#include "global.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


#define EEPROM_WRITE_TIME	1	// 1ms	davidd modify 130821	//10		//10ms

#define	IIC_READ	0x01
#define	IIC_WRITE	0x00

#define M_SCL			P03		// 24C02 SCL
#define M_SDA			P02		// 24C02 SDA
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//extern u8 EEPROM_ADDRESS;

/* Private functions ---------------------------------------------------------*/

#define SetOutputAll()		nop

#define SetInputSCL()		{M_SCL=1;}
#define SetOutputSCL()		nop

#define SetSCL()			{M_SCL=1;}
#define ClrSCL()			{M_SCL=0;}
#define GetSCL()			(M_SCL)

#define SetInputSDA()		{M_SDA=1;}
#define SetOutputSDA()		nop

#define SetSDA()			{M_SDA=1;}
#define ClrSDA()			{M_SDA=0;}
#define GetSDA()			(M_SDA)
		
#define	KtIICDelay()		{ _nop_();_nop_();_nop_();_nop_(); _nop_();_nop_();}		//Delay0_1us(6)
#define	KtIICDelays()		{ _nop_();_nop_(); }

void Init_mI2C(void)
{
	P03_Quasi_Mode;
	P02_Quasi_Mode;
	SetSDA();
	SetSCL();
}

// send START signal. 
void I2C_Start(void)
{
	SetOutputAll();
	SetSDA();
	KtIICDelays();
	SetSCL();
	KtIICDelays();
	ClrSDA();
	KtIICDelays();
	ClrSCL();
	KtIICDelays();
}

// send STOP signal. 
void I2C_Stop(void)
{
//	DBG(("I2C_Stop()\n")); 
	SetOutputAll();
	ClrSDA(); 	
	KtIICDelays();
	SetSCL();
	KtIICDelays();
	SetSDA();
//	SetInputSDA();
}

// send Acknowledgement(ACK or NACK) signal. 
void I2C_SendAck(void)
{
//	DBG(("I2C_SendAck()\n")); 	
	ClrSDA(); 
	KtIICDelays();
	SetSCL();
	KtIICDelay();
	ClrSCL();
	KtIICDelays();
}

// send Acknowledgement(ACK or NACK) signal. 
void I2C_SendNoAck(void)
{
	SetSDA(); 
	KtIICDelays();
	SetSCL();
	KtIICDelay();
	ClrSCL();
	KtIICDelays();
}

// Check Ackowledge signal(ACK/NACK). 
bool I2C_ChkAck(void)
{
	bool	Ack;
	uint8_t i;
	
	ClrSCL();
	SetInputSDA();
	KtIICDelays();
	SetSCL();
	KtIICDelays();
	for (i=0; i<250; i++)
		{
		Ack = !GetSDA();
		if (Ack)
			break;
		}
	KtIICDelays();
	ClrSCL();
	KtIICDelays();
	return Ack;
}

// Send one byte via I2C(check ACK). 
bool I2C_WriteByte(unsigned char Dat)
{
	unsigned char i = 8;

//	DBG(("I2C_WriteByte(%-.2BX)\n", Dat)); 
	SetOutputSDA();	 
	ClrSCL();
	KtIICDelays();
	while(i--)
	{
		if(Dat & 0x80)				//MSB output first
		{
		  	SetSDA(); 
		}
		else
		{
			ClrSDA(); 
		}
		Dat <<= 1;
		KtIICDelays();
		SetSCL();
		KtIICDelay();
		ClrSCL();
		KtIICDelays();
	}
	return I2C_ChkAck();
}

// Receive one byte via I2C. 
unsigned char I2C_ReadByte(void)
{
	unsigned char i = 8;
	unsigned char Dat = 0;

	SetInputSDA();
	KtIICDelays();
	while(i--)
	{	
		Dat <<= 1;
		if(GetSDA())
		{
		  	Dat |= 0x01;
		}
		SetSCL();
		KtIICDelay();
		ClrSCL();
		KtIICDelays();
	}
	SetOutputSDA();
	return Dat;
}



bool I2C_WriteBytes(unsigned char* Buf, unsigned char Len)
{
	while(Len--)
	{
		if (!I2C_WriteByte(*(Buf++)))
		{
			//debug1(("write data err\n"));
			return FALSE;
		}
	}

	return TRUE;
}

// Read data via I2C. 
bool I2C_ReadBytes(unsigned char* Buf, unsigned char Len)
{
	while(Len--)
	{
		*(Buf++) = I2C_ReadByte();
		if(Len == 0)
		{
			I2C_SendNoAck();
		}
		else
		{
			I2C_SendAck();
		}
	}

	return TRUE;
}


// send address via IIC.
bool I2C_SendAddr(
	unsigned char icAddr,
	unsigned int wAddr,
	unsigned char ucRW
	)
{	
	I2C_Start();
	if (!I2C_WriteByte(icAddr))
	{
		Delay_1ms(EEPROM_WRITE_TIME); // Wairt for Programming-time.
		I2C_Start();
		if(!I2C_WriteByte(icAddr))// | PageAddr))
		{
			I2C_Stop();
			return FALSE;
		}
	}
	
   	if(!I2C_WriteByte((unsigned char)wAddr))
	{
	 	I2C_Stop();
		return FALSE;
	}
	
	if (ucRW == IIC_READ)
	{
		I2C_Start();
		if(!I2C_WriteByte(icAddr| IIC_READ))
		{
		 	I2C_Stop();
			return FALSE;
		}
	}
	
	return TRUE;
}

// Write data to eeprom 
bool I2C_MasterWrite(unsigned char device_addr, unsigned char regADD,unsigned char write_data)
{
	bool	acktemp = FALSE;
		
	acktemp = I2C_SendAddr(device_addr, regADD, IIC_WRITE);
	acktemp &= I2C_WriteByte(write_data);
	I2C_Stop();

//	debug1(("I2C_WriteNByte,ACK: %bu\n",(BYTE)acktemp));
	return acktemp;
}

// Read data from eeprom.
bool I2C_MasterRead(unsigned char device_addr,unsigned char regADD, unsigned char* read_data)
{
     bool	acktemp = FALSE;
	device_addr &= 0xFE;
	acktemp = I2C_SendAddr(device_addr,regADD,IIC_READ);
	acktemp &= I2C_ReadBytes(read_data, 1);
	I2C_Stop();

	return acktemp;
}

// Write datas to eeprom from buffer
bool I2C_PolledMasterWrite(unsigned char device_addr, unsigned char regADD,unsigned char* write_data, unsigned char num_bytes)
{
	bool	acktemp = FALSE;
		
	acktemp = I2C_SendAddr(device_addr, regADD, IIC_WRITE);
	acktemp &= I2C_WriteBytes(write_data, num_bytes);
	I2C_Stop();

//	debug1(("I2C_WriteNByte,ACK: %bu\n",(BYTE)acktemp));
	return acktemp;
}

// Read datas from eeprom.
bool I2C_PolledMasterRead(unsigned char device_addr,unsigned char regADD, unsigned char* read_data, unsigned char num_bytes)
{
     bool	acktemp = FALSE;
	device_addr &= 0xFE;
	acktemp = I2C_SendAddr(device_addr,regADD,IIC_READ);
	acktemp &= I2C_ReadBytes(read_data, num_bytes);
	I2C_Stop();

	return acktemp;
}

#if 0
// Write data to ads from buffer	No Register addr
bool  I2C_PolledMasterNWrite(unsigned char device_addr,unsigned char* write_data, unsigned short num_bytes)
{
	bool	acktemp = FALSE;
	
	I2C_Start();
	if (!I2C_WriteByte(device_addr))
	{
		Delay_1ms(EEPROM_WRITE_TIME); // Wairt for Programming-time.
		I2C_Start();
		if(!I2C_WriteByte(device_addr))// | PageAddr))
		{
			I2C_Stop();
			return FALSE;
		}
	}
	
	acktemp &= I2C_WriteBytes(write_data, num_bytes);
	I2C_Stop();

	return acktemp;
}

// Read data from ads7830	No Register addr
bool I2C_PolledMasterNRead(unsigned char device_addr, unsigned char* read_data)
{
	bool	acktemp = FALSE;
	device_addr &= 0xFE;
	I2C_Start();
	if (!I2C_WriteByte(device_addr))
	{
		Delay_1ms(EEPROM_WRITE_TIME); // Wairt for Programming-time.
		I2C_Start();
		if(!I2C_WriteByte(device_addr))
		{
			I2C_Stop();
			return FALSE;
		}
	}
	
	//if (ucRW == IIC_READ)
	{
		I2C_Start();
		if(!I2C_WriteByte(device_addr| IIC_READ))
		{
		 	I2C_Stop();
			return FALSE;
		}
	}
	acktemp &= I2C_ReadBytes(read_data, 1);
	I2C_Stop();

	return acktemp;
}
#endif



