/**********************************************************/
/* i2c_master.h file
/* 
/* Copyright (c) 2002-2005 STMicroelectronics
/*
/***********************************************************/

#ifndef __I2C2_MASTER__
#define __I2C2_MASTER__

#ifdef __cplusplus
extern "C" {
#endif

// defines
#define I2C2_NO_ACK                 0

// functions
void Init_mI2C2(void);
bool I2C2_MasterWrite(unsigned char device_addr, unsigned char regADD,unsigned char write_data);
bool I2C2_MasterRead(unsigned char device_addr,unsigned char regADD, unsigned char* read_data);
bool I2C2_PolledMasterWrite(unsigned char device_addr, unsigned char regADD,unsigned char* write_data, unsigned char num_bytes);
bool I2C2_PolledMasterRead(unsigned char device_addr, unsigned char regADD,unsigned char* read_data, unsigned char num_bytes);
bool  I2C2_PolledMasterNWrite(unsigned char device_addr,unsigned char* write_data, unsigned short num_bytes);
bool I2C2_PolledMasterNRead(unsigned char device_addr, unsigned char* read_data);
#ifdef __cplusplus      /* matches __cplusplus construct above */
}
#endif

#endif // #ifndef __i2c_master

