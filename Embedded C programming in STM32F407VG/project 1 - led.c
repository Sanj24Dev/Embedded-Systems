#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

int main()
{
  //Enabling the AHB1 peripheral clock for GPIOD
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
  //GPIOD configuration
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  
  GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  while(1)
  {
  //GPIOD pin 14 ON
   GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
   for(int i=0;i<5000000;i++);
  }
  
}
