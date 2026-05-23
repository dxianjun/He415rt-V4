
#include "global.h"
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

#define ADDR_TPL0401X		0x5C		// 8bit    _A -- 2E 7bit
#define ADDR_TPL0401B		0x7C		// 8bit	   _B -- 3E 7bit
#define ADDR_TPL0401A		0x5C

bit ReadTplData(unsigned char * rdata)
{
	return I2C_MasterRead(ADDR_TPL0401X,0,rdata);
}

// 参数0-127, 0--输出最大，127输出最小，类似音量寄存器，数值越大，输出越小
bit WriteTplData(unsigned char wdata)
{
	return I2C_MasterWrite(ADDR_TPL0401X,0,wdata);
}

