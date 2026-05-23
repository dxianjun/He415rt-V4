#include "global.h"

const UINT8 BCD_decode_tab[0x10] = { 0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F, 
									 0X77, 0X7C, 0X58, 0X5E, 0X79, 0X71 };//BCD

uint8_t uLedBuf[4]={0};									 

#if 1
extern void I2C_Start(void);
extern void I2C_Stop(void);
extern void I2C_SendNoAck(void);

extern bool I2C_WriteByte(unsigned char Dat);
extern bool I2C_WriteByte(unsigned char Dat);


//#define CH455_I2C_ADDR 	0x40
//#define CH455_I2C_MASK	0x3E
//#define CH455_GET_KEY 	0x07

bool CH455_Write(uint8_t wcmd, uint8_t wdat)	//写数据
{
	bool acktemp = FALSE;
	wcmd = ((wcmd<<1)&0x3E)|0x40;
	//	CH455_I2c_WrByte(((UINT8)(cmd>>7)&CH455_I2C_MASK)|CH455_I2C_ADDR);
	
	clr_EPI;
	I2C_Start();               //启动总线
   
	if (!I2C_WriteByte(wcmd))	
		{
		Delay_1ms(EEPROM_WRITE_TIME); // Wairt for Programming-time.
		I2C_Start();

		if(!I2C_WriteByte(wcmd))
			{
			I2C_Stop();
			return FALSE;
			}
		}

   	acktemp &= I2C_WriteByte(wdat);			//发送数据
  	I2C_Stop();               				//结束总线 
  	clr_PIF;
  	set_EPI;
  	return acktemp;
}

uint8_t CH455_Read(void)		//读取按键
{
	uint8_t keycode;
	uint8_t wcmd;
	wcmd = (CH455_GET_KEY<<1)|0x41;
	//	CH455_I2c_WrByte((UINT8)(CH455_GET_KEY>>7)&CH455_I2C_MASK|0x01|CH455_I2C_ADDR);

	clr_EPI;
	I2C_Start();               //启动总线
	if (!I2C_WriteByte(wcmd))	
		{
		Delay_1ms(EEPROM_WRITE_TIME); // Wairt for Programming-time.
		I2C_Start();

		if(!I2C_WriteByte(wcmd))
			{
			I2C_Stop();
			return FALSE;
			}
		}
	
	keycode=I2C_ReadByte();      //读取数据
	I2C_SendNoAck();
	I2C_Stop();               	//结束总线 
  	clr_PIF;
  	set_EPI;
	return keycode;
}
#endif


void ShowData(uint16_t uData)
{
	uLedBuf[0]= uData/1000%10;
	uLedBuf[1]= uData/100%10;
	uLedBuf[2]= uData/10%10;
	uLedBuf[3]= uData%10;

	CH455_Write(CH455_DIG3,BCD_decode_tab[uLedBuf[0]]); 
	CH455_Write(CH455_DIG2,BCD_decode_tab[uLedBuf[1]]); 
	CH455_Write(CH455_DIG1,BCD_decode_tab[uLedBuf[2]]|0x80); 
	CH455_Write(CH455_DIG0,BCD_decode_tab[uLedBuf[3]]);
}

void Ch455_Sleep(void)
{
	CH455_Write(CH455_SET_PARA, CH455_SYSOFF);	// Off
}

void Ch455_WakeUp(void)
{
	CH455_Write(CH455_SET_PARA, CH455_SYSON_4);	// 4级亮度显示
}

void Ch455_test(void)
{
	uint16_t i;

//	CH455_Write(CH455_SET_PARA,CH455_SYSON);// 开启显示和键盘，8段显示方式
//	CH455_Write(CH455_SET_PARA, CH455_SYSON_8);	// 8级亮度显示
	//发显示数据
	ShowData(1234);

    //亮度变化
    for( i = 1; i < 8; i++ )
		{
	     Delay1ms(500);
 	     CH455_Write(CH455_SET_PARA,CH455_8SEG_ON|i<<4);	// 1-7级亮度显示
    	}
	Delay1ms(500);
	CH455_Write(CH455_SET_PARA, CH455_SYSON_8);	// 8级亮度显示

#if 1
	//4位数码管全部显示0
	ShowData(0);	
	i=0;
	while (1)
		{
		i++;
		if (i>=10000)
			i=0;
		ShowData(i);
		Delay1ms(50);
		}
#endif
}


