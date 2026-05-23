#ifndef __EP24CX_H__
#define __EP24CX_H__

/* Includes ------------------------------------------------------------------*/


#define ADDR_EP24Cxx		0xA0		// 24C02 8bit write address
#define EP24CXX_SIZE		256			// 24C02 total bytes
#define BYTE_PAGE			8			// 24C02 compatible page write size
#define MAX_EROM			32			// Legacy helper data size

// 需要保存在EEPROM的数据
#define DATA_LEN			8

bool ReadEpData(uint16_t raddr,uint8_t *rdata,uint16_t uNum);
bool WriteEpData(uint16_t waddr,uint8_t *wdata,uint16_t uNum);
bool Ep24c02_IsReady(void);
bool Ep24C02_IsMacOk(void);


void TestEp24c512(void);
void TestEp24c32(void);
#endif 







