/**********************************************************/
/* i2c_master.h file
/* 
/* Copyright (c) 2002-2005 STMicroelectronics
/*
/***********************************************************/

#ifndef __I2C_MASTER__
#define __I2C_MASTER__

#ifdef __cplusplus
extern "C" {
#endif

// defines
#define I2C_NO_ACK                 0

// functions
void Init_mI2C(void);
bool I2C_MasterWrite(unsigned char device_addr, unsigned char regADD,unsigned char write_data);
bool I2C_MasterRead(unsigned char device_addr,unsigned char regADD, unsigned char* read_data);
bool I2C_PolledMasterWrite(unsigned char device_addr, unsigned char regADD,unsigned char* write_data, unsigned char num_bytes);
bool I2C_PolledMasterRead(unsigned char device_addr, unsigned char regADD,unsigned char* read_data, unsigned char num_bytes);
bool  I2C_PolledMasterNWrite(unsigned char device_addr,unsigned char* write_data, unsigned short num_bytes);
bool I2C_PolledMasterNRead(unsigned char device_addr, unsigned char* read_data);
#ifdef __cplusplus      /* matches __cplusplus construct above */
}
#endif

#endif // #ifndef __i2c_master

