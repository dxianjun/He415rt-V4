/*--------------------------------------------------------------------------
N76E003 Function_define.h V1.02

All function define inital setting file for Nuvoton N76E003
--------------------------------------------------------------------------*/
#ifndef	__TYPE_DEFINE__
#define __TYPE_DEFINE__

#include <intrins.h>
#include <stdio.h>

#define FSYS_24M		24
#define FSYS_12M		12
#define FSYS_6M			6

#define __FSYS__		FSYS_12M

#if (__FSYS__ == FSYS_12M)
	// 注意T0的时钟，选择Fsys时要*12; 如果选择Fsys/12时，就不用再*12了
    //#define TIMER0_RELOAD_VALUE_1ms     (1000*12)	//(1000*12)/12000000 = 1mS,  		// FT0=FSYS = 12M for TM0
    //#define TIMER0_RELOAD_VALUE_125us   (125*12)	//(1000*12)/12000000 = 1mS,  		// FT0=FSYS = 12M for TM0
    #define TIMER0_RELOAD_VALUE_1ms     (1000)		//(1000*12)/12000000 = 1mS,  			// FT0=FSYS/12 = 1M for TM0
    #define TIMER0_RELOAD_VALUE_125us   (125)		//(1000*12)/12000000 = 1mS,  			// FT0=FSYS/12 = 1M for TM0
    
    #define TIMER1_RELOAD_VALUE_100us   100			//100/1000000 = 100 uS,  			// FT1=FSYS/12 = 12M/12=1M for TM1
	#define TIMER1_RELOAD_VALUE_250us	250 		// 100/1000000 = 200 uS,			// FT1=FSYS/12 = 12M/12=1M for TM1
	#define TIMER1_RELOAD_VALUE_125us	125 		// 125/1000000 = 125 us 

#elif (__FSYS__ == FSYS_24M)
    //#define TIMER0_RELOAD_VALUE_1ms     (1000*24)	//(1000*24)/24000000 = 1mS,  		// FT0=FSYS = 24M for TM0	
    //#define TIMER0_RELOAD_VALUE_125us   (125*24)	//(1000*24)/24000000 = 1mS,  		// FT0=FSYS = 24M for TM0
	#define TIMER0_RELOAD_VALUE_1ms 	(1000*2)	// (1000*2)/2000000 = 1mS,			// FT0=FSYS/12 = 2M for TM0	
    #define TIMER0_RELOAD_VALUE_125us   (125*2)		//(1000*12)/12000000 = 1mS,  		// FT0=FSYS/12 = 2M for TM0
    
    #define TIMER1_RELOAD_VALUE_100us   (100*2)		//100/1000000 = 100 uS,  			// FT1=FSYS/12 = 24M/12=2M for TM1
	#define TIMER1_RELOAD_VALUE_125us	(125*2) 		// 125/1000000 = 125 us 			// FT1=FSYS/12 = 24M/12=2M for TM1
#endif


#define nop _nop_();

typedef bit                   BIT;
typedef unsigned char         UINT8;
typedef unsigned int          UINT16;
typedef unsigned long         UINT32;

typedef unsigned char		uchar;
typedef unsigned int 		uint;

typedef unsigned char		uint8_t;
typedef unsigned int		uint16_t;
typedef unsigned long		uint32_t;

typedef signed long			s32;
typedef signed short		s16;
typedef signed char			s8;

typedef signed long  const sc32;  /* Read Only */
typedef signed short const sc16;  /* Read Only */
typedef signed char  const sc8;   /* Read Only */

typedef volatile signed long  vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef unsigned long  const uc32;  /* Read Only */
typedef unsigned short const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */

typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef enum
{
  FALSE = 0,
  TRUE = !FALSE
}
bool;

//16 --> 8 x 2
#define HIBYTE(v1)              ((uint8_t)((v1)>>8))                      //v1 is uint16_t
#define LOBYTE(v1)              ((uint8_t)((v1)&0xFF))
//8 x 2 --> 16
#define MAKEWORD(v1,v2)         ((((uint16_t)(v1))<<8)+(uint16_t)(v2))      //v1,v2 is uint8_t
//8 x 4 --> 32
#define MAKELONG(v1,v2,v3,v4)   (uint32_t)((v1<<32)+(v2<<16)+(v3<<8)+v4)  //v1,v2,v3,v4 is uint8_t
//32 --> 16 x 2
#define YBYTE1(v1)              ((uint16_t)((v1)>>16))                    //v1 is uint32_t
#define YBYTE0(v1)              ((uint16_t)((v1)&0xFFFF))
//32 --> 8 x 4
#define TBYTE3(v1)              ((uint8_t)((v1)>>24))                     //v1 is uint32_t
#define TBYTE2(v1)              ((uint8_t)((v1)>>16))
#define TBYTE1(v1)              ((uint8_t)((v1)>>8)) 
#define TBYTE0(v1)              ((uint8_t)((v1)&0xFF))

// set1 x|SET_BIT0
#define SET_BIT0        0x01
#define SET_BIT1        0x02
#define SET_BIT2        0x04
#define SET_BIT3        0x08
#define SET_BIT4        0x10
#define SET_BIT5        0x20
#define SET_BIT6        0x40
#define SET_BIT7        0x80
#define SET_BIT8        0x0100
#define SET_BIT9        0x0200
#define SET_BIT10       0x0400
#define SET_BIT11       0x0800
#define SET_BIT12       0x1000
#define SET_BIT13       0x2000
#define SET_BIT14       0x4000
#define SET_BIT15       0x8000

// clr0 x&CLR_BIT0
#define CLR_BIT0        0xFE
#define CLR_BIT1        0xFD
#define CLR_BIT2        0xFB
#define CLR_BIT3        0xF7
#define CLR_BIT4        0xEF
#define CLR_BIT5        0xDF
#define CLR_BIT6        0xBF
#define CLR_BIT7        0x7F

#define CLR_BIT8        0xFEFF
#define CLR_BIT9        0xFDFF
#define CLR_BIT10       0xFBFF
#define CLR_BIT11       0xF7FF
#define CLR_BIT12       0xEFFF
#define CLR_BIT13       0xDFFF
#define CLR_BIT14       0xBFFF
#define CLR_BIT15       0x7FFF

#define FAIL            1
#define PASS            0

/* Bit masks. */
#define BIT0		0x0001
#define BIT1		0x0002
#define BIT2		0x0004
#define BIT3		0x0008
#define BIT4		0x0010
#define BIT5		0x0020
#define BIT6		0x0040
#define BIT7		0x0080
#define BIT8		0x0100
#define BIT9		0x0200
#define BIT10		0x0400
#define BIT11		0x0800
#define BIT12		0x1000
#define BIT13		0x2000
#define BIT14		0x4000
#define BIT15		0x8000
// 32bit 
#define BIT16	0x00010000
#define BIT17	0x00020000
#define BIT18	0x00040000
#define BIT19	0x00080000
#define BIT20	0x00100000
#define BIT21	0x00200000
#define BIT22	0x00400000
#define BIT23	0x00800000
#define BIT24	0x01000000
#define BIT25	0x02000000
#define BIT26	0x04000000
#define BIT27	0x08000000
#define BIT28	0x10000000
#define BIT29	0x20000000
#define BIT30	0x40000000
#define BIT31	0x80000000


/********** 位域（位段）类型定义 **********/
typedef struct {
	unsigned char  b0 : 1;
	unsigned char  b1 : 1;
	unsigned char  b2 : 1;
	unsigned char  b3 : 1;
	unsigned char  b4 : 1;
	unsigned char  b5 : 1;
	unsigned char  b6 : 1;
	unsigned char  b7 : 1;
} bits_t;

typedef union
{
	bits_t        bits;
	unsigned char val;
} bit_field_t;


typedef enum
{
	DATA_NONE = 0,
	DATA_FROM_SUB,
	DATA_FROM_APP
}UartDataType_TypeDef;


typedef struct
{
UartDataType_TypeDef UartDataType;				/* Specifies the Uart data type	*/
unsigned short UartData_Len;					/* Specifies the Uart data package length	*/						
}UartData_TypeDef;


#define MAX_BUF_SIZE 	32
// #define MAX_BUF_NUM 	2

#define USE_UART0		1
#define USE_UART0_ISR 	1


#if USE_UART0_ISR
// uart0
extern uint8_t idata TCP_Data_count0;
extern uint8_t xdata TCP_ISR_buf0[MAX_BUF_SIZE];
#endif

// davidd add 20220621
#define __DBG_EN__ 1
#if 1
#define dPrintf		printf
#define dPrintf_d	printf
#else
#define	dPrintf(SMT) 
#define dPrintf_d
#define dPrintf_Ir(SMT)
#endif
//#define	dPrintf(...)  	// Arm的可以用变长，51的不行

//#define	dPrintf_d		//printf(SMT, (unsigned short)(d))
// davidd end

#endif

