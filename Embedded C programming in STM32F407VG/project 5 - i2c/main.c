// main file
#include "stm32f4xx.h"
#include "cpal_i2c.h"
#include "gyro.h"

uint8_t buff=0;
BSP_GyrFetchTypeDef gyroStruct;

int main()
{  
   if(BSP_Gyroscope_Init() == 0)
     return 0;
  
   BSP_Gyroscope_Read_Register(&buff, 0x75);
   
   if(BSP_Gyroscope_GetValues(&gyroStruct) == ERROR)
     return 0;
   
   while(1); 
}
