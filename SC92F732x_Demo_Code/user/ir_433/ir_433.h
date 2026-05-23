/**********************************************************/
/* ir_433.h file
/* 
/* Copyright (c) 2013 davidd
/*
/***********************************************************/

/******************H头文件*********************************/
 
#ifndef RF433MDecode_H_
#define RF433MDecode_H_
/***************************************************/

//**************************************************//

//Define Constant
//在24us定时中断中调用433M接收解码函数
//同步码，10ms低电平
 
#define C_RF_START_L_MAX        150        //12.4MS   //12.4MS--12400/100 = 124
#define C_RF_START_L_MIN     	40        //6MS	// 6000/100 = 60
 
//之后是25位数据，只取24位3个字节
#define C_RF_REC_BIT_LEN    25    
//"1"是1.1ms高+0.2ms低, "0"是0.45ms高+0.9ms低
//"1"是1.2ms高+0.4ms低, "0"是0.45ms高+1.2ms低
//所以可以只通过高电平判断
 
#define C_RF_DATA1_MAX        15        //1.4ms		1400/100 = 14
#define C_RF_DATA1_MIN        8        	//0.9ms		900/100 = 9
#define C_RF_DATA0_MAX        6	        //0.6ms		600/100 = 6
#define C_RF_DATA0_MIN        1        	//0.3ms		300/100 = 3
 
//结束码，短按：130ms低电平，长按：500ms，这里取大于70ms
//#define C_RF_END_L_MAX        500        //140MS
#define C_RF_END_L_MIN			700        //70MS  70000/100 = 700
#define C_RF_DATA_CLENT_1     0xAD        //客户码1
#define C_RF_DATA_CLENT_2     0x2F        //客户码2

#define C_DELAY_KEY_PRESS_SHORT        500       //1000*1MS		
#define C_DELAY_KEY_PRESS_LONG        2000    	//3000*1MS
/****************************************************/
// Define General Register
 
//**************************************************
//Define FUNCTION
void RF433M_Init(void);
void RF433M_RecevieDecode(void);
void RF433_KeyHandle(void);
void Exit_Study(void);

#endif