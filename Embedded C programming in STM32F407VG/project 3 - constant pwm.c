#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

int main()
{
  //Clock for LED
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
  //GPIO configuration
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  //Enabling timer clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  //Initializing time base structure members
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  //TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period = 99;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 2799;
  //TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  
  //Initializing time base unit peripheral
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
    
  //Initializing OC structure members
  TIM_OCInitTypeDef TIM_OCInitStruct;
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_Pulse = 9;
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  
  
  TIM_OC4Init(TIM4, &TIM_OCInitStruct);
  
  //TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
  
  //TIM_ARRPreloadConfig(TIM3, ENABLE);
  
  //AF for the LED pin
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
  
  TIM_Cmd(TIM4, ENABLE);
      
  while(1);
}
