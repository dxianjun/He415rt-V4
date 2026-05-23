
#ifndef __SFR_MACRO__
#define __SFR_MACRO__

extern bit BIT_TMP;

/**** P0		80H *****/
#define set_P00			P00		=		1
#define set_P01			P01		=		1
#define set_P02			P02		=		1
#define set_P03			P03		=		1
#define set_P04			P04		=		1
#define set_P05			P05		=		1
#define set_P06			P06		=		1
#define set_P07			P07		=		1

#define clr_P00			P00		=		0
#define clr_P01			P01		=		0
#define clr_P02			P02		=		0
#define clr_P03			P03		=		0
#define clr_P04			P04		=		0
#define clr_P05			P05		=		0
#define clr_P06			P06		=		0
#define clr_P07			P07		=		0

//**** SP  	81H **** 
//**** DPH  82H **** 
//**** DPL  83H **** 
//**** RWK  86H **** 

//**** PCON	87H *****
#define set_SMOD    PCON    |= SET_BIT7
#define set_PD      PCON    |= SET_BIT1
#define set_IDL    	PCON    |= SET_BIT0
                            
#define clr_SMOD    PCON    &= ~SET_BIT7
#define clr_PD      PCON    &= ~SET_BIT1
#define clr_IDL    	PCON    &= ~SET_BIT0

/**** TCON		88H ****/
#define set_TF1			TF1		=		1
#define set_TR1			TR1		=		1
#define set_TF0			TF0		=		1
#define set_TR0			TR0		=		1


#define clr_TF1			TF1		=		0
#define clr_TR1			TR1		=		0
#define clr_TF0			TF0		=		0
#define clr_TR0			TR0		=		0


//**** TMOD		89H ****  ok
#define set_T1_CT		TMOD   	|= 	SET_BIT6
#define set_T1_M1		TMOD  	|= 	SET_BIT5
#define set_T1_M0		TMOD   	|= 	SET_BIT4
                         
#define clr_T1_CT		TMOD   	&= 	~SET_BIT6
#define clr_T1_M1		TMOD   	&= 	~SET_BIT5
#define clr_T1_M0 		TMOD   	&= 	~SET_BIT4

#define set_T0_CT		TMOD 	|= 	SET_BIT2
#define set_T0_M1		TMOD   	|= 	SET_BIT1
#define set_T0_M0		TMOD   	|= 	SET_BIT0

#define clr_T0_CT		TMOD   	&= 	~SET_BIT2
#define clr_T0_M1		TMOD   	&= 	~SET_BIT1
#define clr_T0_M0		TMOD   	&= 	~SET_BIT0

#define set_T0_Mode0	TMOD &=0xFC
#define set_T0_Mode1	{TMOD&=0xFC; TMOD|=0x01;}
#define set_T0_Mode2	{TMOD&=0xFC; TMOD|=0x02;}
#define set_T0_Mode3	{TMOD&=0xFC; TMOD|=0x03;}

#define set_T1_Mode0	TMOD &=0xCF
#define set_T1_Mode1	{TMOD&=0xCF; TMOD|=0x10;}
#define set_T1_Mode2	{TMOD&=0xCF; TMOD|=0x20;}
/*
M[1:0]
T1 的模式
00 ： 13 位定时器/计数器，TL1 高 3 位无效
01 ： 16 位定时器/计数器，TL1 和 TH1 全
10 ： 8 位自动重载定时器，溢出时将 TH1 存放的值自动重装入 TL1
11 ： 定时器/计数器 1 无效(停止计数)

T0 的模式
00 ： 13 位定时器/计数器，TL0 高 3 位无效
01 ： 16 位定时器/计数器，TL0 和 TH0 全
10 ： 8 位自动重载定时器，溢出时将 TH0 存放的值自动重装入 TL0
11 ： 定时器 0 此时作为双 8 位定时器/计数器。TL0 作为一个 8 位定时器/计数器，通过标准定时器 0 的控制位控制；TH0 仅作为一个 8 位定时器，由定时器 1 的控制位控制。
*/
//**** TH1		8AH **** 
//**** TH0		8BH **** 
//**** TL1		8CH	**** 
//**** TL0		8DH **** 

//**** TMCON	8EH **** ok
// 时钟源选择为系统时钟-Fsys
#define set_Fsys_T0	TMCON |= SET_BIT0
#define set_Fsys_T1	TMCON |= SET_BIT1
#define set_Fsys_T2	TMCON |= SET_BIT2
// 时钟源选择为1/12系统时钟-Fsys/12, 与标准8051兼容
#define clr_Fsys_T0	TMCON &= ~SET_BIT0
#define clr_Fsys_T1	TMCON &= ~SET_BIT1
#define clr_Fsys_T2	TMCON &= ~SET_BIT2

/**** P1		90H *****/
#define set_P10			P10		=		1
#define set_P11			P11		=		1
#define set_P12			P12		=		1
#define set_P13			P13		=		1
#define set_P14			P14		=		1
#define set_P15			P15		=		1
#define set_P16			P16		=		1
#define set_P17			P17		=		1

#define clr_P10			P10		=		0
#define clr_P11			P11		=		0
#define clr_P12			P12		=		0
#define clr_P13			P13		=		0
#define clr_P14			P14		=		0
#define clr_P15			P15		=		0
#define clr_P16			P16		=		0
#define clr_P17			P17		=		0


//**** SCON		98H ****
#define set_SM0     SM0   = 1
#define set_SM1     SM1   = 1
#define set_SM2     SM2   = 1
#define set_REN     REN   = 1
#define set_TB8     TB8   = 1
#define set_RB8     RB8   = 1
#define set_TI      TI    = 1
#define set_RI      RI    = 1

#define clr_SM0     SM0   = 0
#define clr_SM1     SM1   = 0
#define clr_SM2     SM2   = 0
#define clr_REN     REN   = 0
#define clr_TB8     TB8   = 0
#define clr_RB8     RB8   = 0
#define clr_TI      TI    = 0
#define clr_RI      RI    = 0

#define set_Uart_Mode1	{clr_SM0; set_SM1;}

//**** SBUF		99H ****
//**** SBUF_1	9AH ****
                          
//**** P2		A0H ****

//**** IAPAL	A6H ****
//**** IAPAH	A7H ****

//**** IE			A8H ****
#define set_EA      EA       = 1
#define set_EADC    EADC     = 1
#define set_ET2    	ET2      = 1
#define set_EUART   EUART    = 1
#define set_ET1     ET1      = 1
#define set_EINT1   EINT1    = 1
#define set_ET0     ET0      = 1
#define set_EINT0   EINT0    = 1

#define clr_EA      EA       = 0
#define clr_EADC    EADC     = 0
#define clr_ET2     ET2      = 0
#define clr_EUART      EUART    = 0
#define clr_ET1     ET1      = 0
#define clr_EINT1   EINT1    = 0
#define clr_ET0     ET0      = 0
#define clr_EINT0   EINT0    = 0

//**** IE1			A9H ****
/*IE1*/  // 不能按位寻址
//IE1.3 INT2中断使能控制位	0:关闭INT0中断      1:打开INT0中断
//IE1.2 BTM中断使能控制位	0:关闭BTM中断       1:打开BTM中断
//IE1.1	PWM中断使能控制位   0:关闭PWM中断       1:打开PWM中断

#define set_EINT2 	IE1 |= SET_BIT3
#define set_EBTM		IE1 |= SET_BIT2
#define set_EPWM		IE1 |= SET_BIT1

#define clr_EINT2 	IE1 &= ~SET_BIT3
#define clr_EBTM		IE1 &= ~SET_BIT2
#define clr_EPWM		IE1 &= ~SET_BIT1


//**** IP		B8H	****
#define set_IPADC   IPADC	= 1
#define set_IPT2    IPT2	= 1
#define set_IPS     IPUART	= 1
#define set_IPT1    IPT1	= 1
#define set_IPX1    IPINT1	= 1
#define set_IPT0    IPT0	= 1
#define set_IPX0    IPINT0	= 1

#define clr_IPADC	IPADC	= 0
#define clr_IPT2	IPT2	= 0
#define clr_IPS		IPUART	= 0
#define clr_IPT1	IPT1	= 0
#define clr_IPX1	IPINT1	= 0
#define clr_IPT0	IPT0	= 0
#define clr_IPX0	IPINT0	= 0

//**** IP1		B9H	****

/*IP1*/ // 不能按位寻址
//IP1.3 INT2中断优先控制位	   0:设定 INT2中断优先权是 “低”		    1:设定 INT2的中断优先权是 “高”
//IP1.2 BTM中断优先控制位	   0:设定 BTM中断优先权是 “低”		    1:设定 BTM的中断优先权是 “高”
//IP1.1 PWM中断优先控制位      0:设定 PWM的中断优先权是 “低”			1:设定 PWM的中断优先权是 “高”
#define set_IPINT2	IP1 |= SET_BIT3  
#define set_IPBTM	IP1 |= SET_BIT2 
#define set_IPPWM	IP1 |= SET_BIT1

#define clr_IPINT2	IP1 &= ~SET_BIT3  
#define clr_IPBTM	IP1 &= ~SET_BIT2 
#define clr_IPPWM	IP1 &= ~SET_BIT1


//**** ADCRL		C2H ****
//**** ADCRH		C3H ****

//**** T2CON  C8H ****
#define set_TF2     TF2      = 1
#define set_TR2     TR2      = 1


#define clr_TF2     TF2      = 0
#define clr_TR2     TR2      = 0

#define set_baud_from_T1	{RCLK=0;TCLK=0;}
#define set_baud_from_T2	{RCLK=1;TCLK=1;}

//**** T2MOD	C9H ****                     
#define set_LDEN    T2MOD   |= SET_BIT7
#define set_T2DIV2  T2MOD   |= SET_BIT6
#define set_T2DIV1  T2MOD   |= SET_BIT5
#define set_T2DIV0  T2MOD   |= SET_BIT4
#define set_CAPCR   T2MOD   |= SET_BIT3
#define set_CMPCR   T2MOD   |= SET_BIT2
#define set_LDTS1   T2MOD   |= SET_BIT1
#define set_LDTS0   T2MOD   |= SET_BIT0
                            
#define clr_LDEN    T2MOD   &= ~SET_BIT7
#define clr_T2DIV2  T2MOD   &= ~SET_BIT6
#define clr_T2DIV1  T2MOD   &= ~SET_BIT5
#define clr_T2DIV0  T2MOD   &= ~SET_BIT4
#define clr_CAPCR   T2MOD   &= ~SET_BIT3
#define clr_CMPCR   T2MOD   &= ~SET_BIT2
#define clr_LDTS1   T2MOD   &= ~SET_BIT1
#define clr_LDTS0   T2MOD   &= ~SET_BIT0

//**** RCAP2L CAH ****
//**** RCAP2H CBH ****
//**** TL2	CCH  ****
//**** TH2	CDH  ****

//**** WDTCON		CFH **** 看门狗
#define set_WDCLR   WDTCON|=SET_BIT4


/****  PSW 		D0H ****/
#define set_CY   	CY	= 1
#define set_AC		AC  = 1
#define set_F0    	F0	= 1 
#define set_RS1		RS1 = 1
#define set_RS0		RS0 = 1
#define set_OV		OV 	= 1
#define set_P			P		= 1

#define clr_CY   	CY	= 0
#define clr_AC		AC  = 0
#define clr_F0    	F0	= 0 
#define clr_RS1		RS1 = 0
#define clr_RS0		RS0 = 0
#define clr_OV		OV 	= 0
#define clr_P			P		= 0

 
//**** PWMCFG0		D1H ****
//**** PWMCFG1		D4H ****

//**** PWMCON		D2H *** ok
#define set_PWMRUN  PWMCON  |= SET_BIT7
#define set_PWMF    PWMCON	|= SET_BIT6
#define set_ENPWM0	PWMCON	|= SET_BIT3
#define set_ENPWM1	PWMCON	|= SET_BIT4
#define set_ENPWM2	PWMCON	|= SET_BIT5

#define clr_PWMRUN  PWMCON  &= ~SET_BIT7
#define clr_PWMF    PWMCON	&= ~SET_BIT6
#define clr_ENPWM0	PWMCON	&= ~SET_BIT3
#define clr_ENPWM1	PWMCON	&= ~SET_BIT4
#define clr_ENPWM2	PWMCON	&= ~SET_BIT5

//**** PWMPH		D1H ****
//**** PWM0H		D2H ****
//**** PWM1H		D3H ****
//**** PWM2H		D4H ****
//**** PWM3H		D5H	****

#if 0
//**** PDTEN		F9H ****
#define set_PDT45EN PDTEN|= SET_BIT2
#define set_PDT23EN PDTEN|= SET_BIT1
#define set_PDT01EN PDTEN|= SET_BIT0

#define clr_PDT45EN PDTEN &= ~SET_BIT2
#define clr_PDT23EN PDTEN &= ~SET_BIT1
#define clr_PDT01EN PDTEN &= ~SET_BIT0
#endif

/*****************************************************************************************
* For PWM setting 
*****************************************************************************************/
//--------- PMW clock source select define ---------------------
#define		PWM_CLOCK_FSYS					PWMCON&=0xF8
//--------- PWM clock devide define ----------------------------
#define		PWM_CLOCK_DIV_1					PWMCON&=0xF8
#define		PWM_CLOCK_DIV_2					PWMCON|=0x01;PWMCON&=0xF9
#define		PWM_CLOCK_DIV_4					PWMCON|=0x02;PWMCON&=0xFA
#define		PWM_CLOCK_DIV_8					PWMCON|=0x03;PWMCON&=0xFB
#define		PWM_CLOCK_DIV_32				PWMCON|=0x04;PWMCON&=0xFC
#define		PWM_CLOCK_DIV_64				PWMCON|=0x05;PWMCON&=0xFD
#define		PWM_CLOCK_DIV_128				PWMCON|=0x06;PWMCON&=0xFE
#define		PWM_CLOCK_DIV_256				PWMCON|=0x07
//--------- PWM I/O select define ------------------------------
#define		PWM0_P00_OUTPUT_ENABLE			PWMCON|=SET_BIT3
#define		PWM1_P01_OUTPUT_ENABLE			PWMCON|=SET_BIT4
#define		PWM2_P02_OUTPUT_ENABLE			PWMCON|=SET_BIT5

#define		PWM0_P00_OUTPUT_DISABLE			PWMCON&= ~SET_BIT3
#define		PWM1_P01_OUTPUT_DISABLE			PWMCON&= ~SET_BIT4
#define		PWM2_P02_OUTPUT_DISABLE			PWMCON&= ~SET_BIT5


#if ((IC_MODEL == SC92F7351)||(IC_MODEL == SC92F7352)||(IC_MODEL == SC92F7350))
//--------- PWM I/O Polarity Control ---------------------------				
#define		PWM0_OUTPUT_INVERSE			PWMCFG0|=SET_BIT3
#define		PWM1_OUTPUT_INVERSE			PWMCFG0|=SET_BIT4
#define		PWM2_OUTPUT_INVERSE			PWMCFG0|=SET_BIT5
#define		PWM_OUTPUT_ALL_INVERSE		PWMCFG0=0x38
				
#define		PWM0_OUTPUT_NORMAL			PWMCFG0&= ~SET_BIT3
#define		PWM1_OUTPUT_NORMAL			PWMCFG0&= ~SET_BIT4	
#define		PWM2_OUTPUT_NORMAL			PWMCFG0&= ~SET_BIT5	
#define		PWM_OUTPUT_ALL_NORMAL		PWMCFG0&= ~0x38
#else
//--------- PWM I/O Polarity Control ---------------------------				
#define		PWM0_OUTPUT_INVERSE			PWMCFG|=SET_BIT3
#define		PWM1_OUTPUT_INVERSE			PWMCFG|=SET_BIT4
#define		PWM2_OUTPUT_INVERSE			PWMCFG|=SET_BIT5
#define		PWM_OUTPUT_ALL_INVERSE		PWMCFG=0x38
				
#define		PWM0_OUTPUT_NORMAL			PWMCFG&= ~SET_BIT3
#define		PWM1_OUTPUT_NORMAL			PWMCFG&= ~SET_BIT4	
#define		PWM2_OUTPUT_NORMAL			PWMCFG&= ~SET_BIT5	
#define		PWM_OUTPUT_ALL_NORMAL		PWMCFG&= ~0x38

#define		PWM0_P25_OUTPUT_ENABLE		PWMCFG|=SET_BIT0
#define		PWM1_P26_OUTPUT_ENABLE		PWMCFG|=SET_BIT1
#define		PWM2_P27_OUTPUT_ENABLE		PWMCFG|=SET_BIT2
#endif

#if 1
// 7531 有PWM3-5,  7321只有PWM0-2, 所以PWMCFG1的位置功能不同;


#endif

/*****************************************************************************************
* For IO setting 
*****************************************************************************************/

//------------------- Define Port as Input Only mode ---------- reset mode ---------
//
#define P0_Input_Mode			P0CON=0x00;P0PH=0x00
#define P00_Input_Mode			P0CON&=~SET_BIT0;P0PH&=~SET_BIT0
#define P01_Input_Mode			P0CON&=~SET_BIT1;P0PH&=~SET_BIT1
#define P02_Input_Mode			P0CON&=~SET_BIT2;P0PH&=~SET_BIT2
#define P03_Input_Mode			P0CON&=~SET_BIT3;P0PH&=~SET_BIT3
#define P04_Input_Mode			P0CON&=~SET_BIT4;P0PH&=~SET_BIT4
#define P05_Input_Mode			P0CON&=~SET_BIT5;P0PH&=~SET_BIT5
#define P06_Input_Mode			P0CON&=~SET_BIT6;P0PH&=~SET_BIT6
#define P07_Input_Mode			P0CON&=~SET_BIT7;P0PH&=~SET_BIT7

#define P1_Input_Mode			P1CON=0x00;P1PH=0x00
#define P10_Input_Mode			P1CON&=~SET_BIT0;P1PH&=~SET_BIT0
#define P11_Input_Mode			P1CON&=~SET_BIT1;P1PH&=~SET_BIT1
#define P12_Input_Mode			P1CON&=~SET_BIT2;P1PH&=~SET_BIT2
#define P13_Input_Mode			P1CON&=~SET_BIT3;P1PH&=~SET_BIT3
#define P14_Input_Mode			P1CON&=~SET_BIT4;P1PH&=~SET_BIT4
#define P15_Input_Mode			P1CON&=~SET_BIT5;P1PH&=~SET_BIT5
#define P16_Input_Mode			P1CON&=~SET_BIT6;P1PH&=~SET_BIT6
#define P17_Input_Mode			P1CON&=~SET_BIT7;P1PH&=~SET_BIT7

#define P2_Input_Mode			P2CON=0x00;P2PH=0x00
#define P20_Input_Mode			P2CON&=~SET_BIT0;P2PH&=~SET_BIT0
#define P21_Input_Mode			P2CON&=~SET_BIT1;P2PH&=~SET_BIT1
#define P22_Input_Mode			P2CON&=~SET_BIT2;P2PH&=~SET_BIT2
#define P23_Input_Mode			P2CON&=~SET_BIT3;P2PH&=~SET_BIT3
#define P24_Input_Mode			P2CON&=~SET_BIT4;P2PH&=~SET_BIT4
#define P25_Input_Mode			P2CON&=~SET_BIT5;P2PH&=~SET_BIT5
#define P26_Input_Mode			P2CON&=~SET_BIT6;P2PH&=~SET_BIT6
#define P27_Input_Mode			P2CON&=~SET_BIT7;P2PH&=~SET_BIT7


//------------------- Define Port as Quasi mode  ------------Ipu mode-------
#define P0_Quasi_Mode			P0CON=0x00;P0PH=0xFF
#define P00_Quasi_Mode			P0CON&=~SET_BIT0;P0PH|=SET_BIT0
#define P01_Quasi_Mode			P0CON&=~SET_BIT1;P0PH|=SET_BIT1
#define P02_Quasi_Mode			P0CON&=~SET_BIT2;P0PH|=SET_BIT2
#define P03_Quasi_Mode			P0CON&=~SET_BIT3;P0PH|=SET_BIT3
#define P04_Quasi_Mode			P0CON&=~SET_BIT4;P0PH|=SET_BIT4
#define P05_Quasi_Mode			P0CON&=~SET_BIT5;P0PH|=SET_BIT5
#define P06_Quasi_Mode			P0CON&=~SET_BIT6;P0PH|=SET_BIT6
#define P07_Quasi_Mode			P0CON&=~SET_BIT7;P0PH|=SET_BIT7

#define P1_Quasi_Mode			P1CON=0x00;P1PH=0xFF
#define P10_Quasi_Mode			P1CON&=~SET_BIT0;P1PH|=SET_BIT0
#define P11_Quasi_Mode			P1CON&=~SET_BIT1;P1PH|=SET_BIT1
#define P12_Quasi_Mode			P1CON&=~SET_BIT2;P1PH|=SET_BIT2
#define P13_Quasi_Mode			P1CON&=~SET_BIT3;P1PH|=SET_BIT3
#define P14_Quasi_Mode			P1CON&=~SET_BIT4;P1PH|=SET_BIT4
#define P15_Quasi_Mode			P1CON&=~SET_BIT5;P1PH|=SET_BIT5
#define P16_Quasi_Mode			P1CON&=~SET_BIT6;P1PH|=SET_BIT6
#define P17_Quasi_Mode			P1CON&=~SET_BIT7;P1PH|=SET_BIT7

#define P2_Quasi_Mode			P2CON=0x00;P2PH=0xFF
#define P20_Quasi_Mode			P2CON&=~SET_BIT0;P2PH|=SET_BIT0
#define P21_Quasi_Mode			P2CON&=~SET_BIT1;P2PH|=SET_BIT1
#define P22_Quasi_Mode			P2CON&=~SET_BIT2;P2PH|=SET_BIT2
#define P23_Quasi_Mode			P2CON&=~SET_BIT3;P2PH|=SET_BIT3
#define P24_Quasi_Mode			P2CON&=~SET_BIT4;P2PH|=SET_BIT4
#define P25_Quasi_Mode			P2CON&=~SET_BIT5;P2PH|=SET_BIT5
#define P26_Quasi_Mode			P2CON&=~SET_BIT6;P2PH|=SET_BIT6
#define P27_Quasi_Mode			P2CON&=~SET_BIT7;P2PH|=SET_BIT7

//------------------- Define Port as Push Pull mode -------------------
#define P0_PushPull_Mode		P0CON = 0xFF
#define P00_PushPull_Mode		P0CON |= SET_BIT0
#define P01_PushPull_Mode		P0CON |= SET_BIT1
#define P02_PushPull_Mode		P0CON |= SET_BIT2
#define P03_PushPull_Mode		P0CON |= SET_BIT3
#define P04_PushPull_Mode		P0CON |= SET_BIT4
#define P05_PushPull_Mode		P0CON |= SET_BIT5
#define P06_PushPull_Mode		P0CON |= SET_BIT6
#define P07_PushPull_Mode		P0CON |= SET_BIT7

#define P1_PushPull_Mode		P1CON = 0xFF
#define P10_PushPull_Mode		P1CON |= SET_BIT0
#define P11_PushPull_Mode		P1CON |= SET_BIT1
#define P12_PushPull_Mode		P1CON |= SET_BIT2
#define P13_PushPull_Mode		P1CON |= SET_BIT3
#define P14_PushPull_Mode		P1CON |= SET_BIT4
#define P15_PushPull_Mode		P1CON |= SET_BIT5
#define P16_PushPull_Mode		P1CON |= SET_BIT6
#define P17_PushPull_Mode		P1CON |= SET_BIT7

#define P2_PushPull_Mode		P2CON = 0xFF
#define P20_PushPull_Mode		P2CON |= SET_BIT0
#define P21_PushPull_Mode		P2CON |= SET_BIT1
#define P22_PushPull_Mode		P2CON |= SET_BIT2
#define P23_PushPull_Mode		P2CON |= SET_BIT3
#define P24_PushPull_Mode		P2CON |= SET_BIT4
#define P25_PushPull_Mode		P2CON |= SET_BIT5
#define P26_PushPull_Mode		P2CON |= SET_BIT6
#define P27_PushPull_Mode		P2CON |= SET_BIT7


#endif


