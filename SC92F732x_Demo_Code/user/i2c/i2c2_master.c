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

#define M2_SCL			P13		// 
#define M2_SDA			P14		// 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//extern u8 EEPROM_ADDRESS;

/* Private functions ---------------------------------------------------------*/

#define SetOutputAll2()		nop

#define SetInputSCL2()		{M2_SCL=1;}
#define SetOutputSCL2()		nop

#define SetSCL2()			{M2_SCL=1;}
#define ClrSCL2()			{M2_SCL=0;}
#define GetSCL2()			(M2_SCL)

#define SetInputSDA2()		{M2_SDA=1;}
#define SetOutputSDA2()		nop

#define SetSDA2()			{M2_SDA=1;}
#define ClrSDA2()			{M2_SDA=0;}
#define GetSDA2()			(M2_SDA)
		
#define	KtIIC2Delay()		{ _nop_();_nop_();_nop_();_nop_(); _nop_();_nop_();}		//Delay0_1us(6)
#define	KtIIC2Delays()		{ _nop_();_nop_(); }

void Init_mI2C2(void)
{
	P13_Quasi_Mode;
	P14_Quasi_Mode;

	P13 = 1;		// 输出高电平
//	set_P1S_3;	

	P14 = 1;		// 输入上拉
	set_P1S_4;	
}

// send START signal. 
void I2C2_Start(void)
{
	SetOutputAll2();
	SetSDA2();
	KtIIC2Delays();
	SetSCL2();
	KtIIC2Delays();
	ClrSDA2();
	KtIIC2Delays();
	ClrSCL2();
	KtIIC2Delays();
}

// send STOP signal. 
void I2C2_Stop(void)
{
//	DBG(("I2C_Stop()\n")); 
	SetOutputAll2();
	ClrSDA2(); 	
	KtIIC2Delays();
	SetSCL2();
	KtIIC2Delays();
	SetSDA2();
	SetInputSDA2();
}

// send Acknowledgement(ACK or NACK) signal. 
void I2C2_SendAck(void)
{
//	DBG(("I2C_SendAck()\n")); 	
	ClrSDA2(); 
	KtIIC2Delays();
	SetSCL2();
	KtIIC2Delay();
	ClrSCL2();
	KtIIC2Delays();
}

// send Acknowledgement(ACK or NACK) signal. 
void I2C2_SendNoAck(void)
{
	SetSDA2(); 
	KtIIC2Delays();
	SetSCL2();
	KtIIC2Delay();
	ClrSCL2();
	KtIIC2Delays();
}

// Check Ackowledge signal(ACK/NACK). 
bool I2C2_ChkAck(void)
{
	unsigned char idata i=0;
	bool	Ack;

	ClrSCL2();
	SetInputSDA2();
	KtIIC2Delays();
	SetSCL2();
#if 0
	for(i=0;i<10;i++)
		{
		KtIIC2Delays();
		Ack = !GetSDA2();
		if (Ack)
			break;
		}
#else
	KtIIC2Delays();
	Ack = !GetSDA2();
#endif

	KtIIC2Delays();
	ClrSCL2();
	KtIIC2Delays();
	return Ack;
}

// Send one byte via I2C(check ACK). 
bool I2C2_WriteByte(unsigned char Dat)
{
	unsigned char i = 8;

//	DBG(("I2C_WriteByte(%-.2BX)\n", Dat)); 
	SetOutputSDA2();	 
	ClrSCL2();
	KtIIC2Delays();
	while(i--)
	{
		if(Dat & 0x80)				//MSB output first
		{
		  	SetSDA2(); 
		}
		else
		{
			ClrSDA2(); 
		}
		Dat <<= 1;
		KtIIC2Delays();
		SetSCL2();
		KtIIC2Delay();
		ClrSCL2();
		KtIIC2Delays();
	}
	return I2C2_ChkAck();
}

// Receive one byte via I2C. 
unsigned char I2C2_ReadByte(void)
{
	unsigned char i = 8;
	unsigned char Dat = 0;

	SetInputSDA2();
	KtIIC2Delays();
	while(i--)
	{	
		Dat <<= 1;
		if(GetSDA2())
		{
		  	Dat |= 0x01;
		}
		SetSCL2();
		KtIIC2Delay();
		ClrSCL2();
		KtIIC2Delays();
	}
	SetOutputSDA2();
	return Dat;
}



bool I2C2_WriteBytes(unsigned char* Buf, unsigned char Len)
{
	while(Len--)
	{
		if (!I2C2_WriteByte(*(Buf++)))
		{
			//debug1(("write data err\n"));
			return FALSE;
		}
	}

	return TRUE;
}

// Read data via I2C. 
bool I2C2_ReadBytes(unsigned char* Buf, unsigned char Len)
{
	while(Len--)
	{
		*(Buf++) = I2C2_ReadByte();
		if(Len == 0)
		{
			I2C2_SendNoAck();
		}
		else
		{
			I2C2_SendAck();
		}
	}

	return TRUE;
}


// send address via IIC.
bool I2C2_SendAddr(
	unsigned char icAddr,
	unsigned int wAddr,
	unsigned char ucRW
	)
{	
	I2C2_Start();
	if (!I2C2_WriteByte(icAddr))
	{
		Delay1ms(EEPROM_WRITE_TIME); // Wairt for Programming-time.
		I2C2_Start();
		if(!I2C2_WriteByte(icAddr))// | PageAddr))
		{
			I2C2_Stop();
			return FALSE;
		}
	}
	
   	if(!I2C2_WriteByte((unsigned char)wAddr))
	{
	 	I2C2_Stop();
		return FALSE;
	}
	
	if (ucRW == IIC_READ)
	{
		I2C2_Start();
		if(!I2C2_WriteByte(icAddr| IIC_READ))
		{
		 	I2C2_Stop();
			return FALSE;
		}
	}
	
	return TRUE;
}

// Write data to eeprom 
bool I2C2_MasterWrite(unsigned char device_addr, unsigned char regADD,unsigned char write_data)
{
	bool	acktemp = FALSE;
		
	acktemp = I2C2_SendAddr(device_addr, regADD, IIC_WRITE);
	acktemp &= I2C2_WriteByte(write_data);
	I2C2_Stop();

//	debug1(("I2C2_WriteNByte,ACK: %bu\n",(BYTE)acktemp));
	return acktemp;
}

// Read data from eeprom.
bool I2C2_MasterRead(unsigned char device_addr,unsigned char regADD, unsigned char* read_data)
{
     bool	acktemp = FALSE;
	device_addr &= 0xFE;
	acktemp = I2C2_SendAddr(device_addr,regADD,IIC_READ);
	acktemp &= I2C2_ReadBytes(read_data, 1);
	I2C2_Stop();

	return acktemp;
}

// Write datas to eeprom from buffer
bool I2C2_PolledMasterWrite(unsigned char device_addr, unsigned char regADD,unsigned char* write_data, unsigned char num_bytes)
{
	bool	acktemp = FALSE;
		
	acktemp = I2C2_SendAddr(device_addr, regADD, IIC_WRITE);
	acktemp &= I2C2_WriteBytes(write_data, num_bytes);
	I2C2_Stop();

//	debug1(("I2C2_WriteNByte,ACK: %bu\n",(BYTE)acktemp));
	return acktemp;
}

// Read datas from eeprom.
bool I2C2_PolledMasterRead(unsigned char device_addr,unsigned char regADD, unsigned char* read_data, unsigned char num_bytes)
{
     bool	acktemp = FALSE;
	device_addr &= 0xFE;
	acktemp = I2C2_SendAddr(device_addr,regADD,IIC_READ);
	acktemp &= I2C2_ReadBytes(read_data, num_bytes);
	I2C2_Stop();

	return acktemp;
}

// Write data to ads from buffer	No Register addr
bool  I2C2_PolledMasterNWrite(unsigned char device_addr,unsigned char* write_data, unsigned short num_bytes)
{
	bool	acktemp = FALSE;
	
	I2C2_Start();
	if (!I2C2_WriteByte(device_addr))
	{
		Delay1ms(EEPROM_WRITE_TIME); // Wairt for Programming-time.
		I2C2_Start();
		if(!I2C2_WriteByte(device_addr))// | PageAddr))
		{
			I2C2_Stop();
			return FALSE;
		}
	}
	
	acktemp &= I2C2_WriteBytes(write_data, num_bytes);
	I2C2_Stop();

	return acktemp;
}

// Read data from ads7830	No Register addr
bool I2C2_PolledMasterNRead(unsigned char device_addr, unsigned char* read_data)
{
	bool	acktemp = FALSE;
	device_addr &= 0xFE;
	I2C2_Start();
	if (!I2C2_WriteByte(device_addr))
	{
		Delay1ms(EEPROM_WRITE_TIME); // Wairt for Programming-time.
		I2C2_Start();
		if(!I2C2_WriteByte(device_addr))
		{
			I2C2_Stop();
			return FALSE;
		}
	}
	
	//if (ucRW == IIC_READ)
	{
		I2C2_Start();
		if(!I2C2_WriteByte(device_addr| IIC_READ))
		{
		 	I2C2_Stop();
			return FALSE;
		}
	}
	acktemp &= I2C2_ReadBytes(read_data, 1);
	I2C2_Stop();

	return acktemp;
}


