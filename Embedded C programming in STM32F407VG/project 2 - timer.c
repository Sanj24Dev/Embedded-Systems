#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

 int main()
{
  //Enabling timer clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  //Initializing time base structure members
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period = 9999;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 2799;
  //TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  
  //timer output is 1000Hz
  //time in seconds 10s
  //Initializing time base unit peripheral
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
  
  //Initializing NVIC structure members
  NVIC_InitTypeDef NVIC_InitStruct;
  NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  
  //Enabling the NVIC 
  NVIC_Init(&NVIC_InitStruct);
  
  //Enabling the clock for GPIO
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  GPIO_SetBits(GPIOD, GPIO_Pin_15);
  
  //TIM_ARRPreloadConfig(TIM3, ENABLE);
  //Enabling the corresponding interrupt
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  //TIM_SetCounter(TIM4, 0);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  
  
  TIM_Cmd(TIM4, ENABLE);
  while(1);
}
