#include "stm32f4xx.h"
#include "cpal_i2c.h"
#include "cpal.h"

#define SELF_TEST_X 0x0D
#define SELF_TEST_Y 0x0E
#define SELF_TEST_Z 0x0F
#define TIMER_TIMEOUT_I2C 0x000FFFFF

typedef struct
{
	double BSP_Acc_x;
	double BSP_Acc_y;
	double BSP_Acc_z;
	double BSP_Temp;
	double BSP_Gyr_x;
	double BSP_Gyr_y;
	double BSP_Gyr_z;
}BSP_GyrFetchTypeDef;



uint32_t BSP_Gyroscope_Init();
void BSP_Gyroscope_Read_WhoAmI(uint8_t*);
void BSP_Gyroscope_Read_Register(uint8_t*, uint8_t);
uint8_t BSP_Gyroscope_SelfTest();
uint8_t BSP_Gyroscope_GetValues(BSP_GyrFetchTypeDef*);
