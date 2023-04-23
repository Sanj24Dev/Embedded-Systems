#include "stm32f4xx.h"

// tim1 ch1 == pe9, pa8
// tim1 ch2 == pa9, pe11
// tim1 ch3 == pa10



int ch1=5,ch2=50,ch3=100;
TIM_OCInitTypeDef TIM_OCStruct1, TIM_OCStruct2, TIM_OCStruct3;

// irq for varying pwm
void TIM1_CC_IRQHandler()
{
	if(TIM_GetITStatus(TIM1,TIM_IT_CC1) !=RESET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);
		TIM_OCStruct1.TIM_Pulse += ch1;
    if(TIM_OCStruct1.TIM_Pulse > 9999)
      TIM_OCStruct1.TIM_Pulse = 0;
    TIM_OC1Init(TIM1, &TIM_OCStruct1);
	}
  if(TIM_GetITStatus(TIM1,TIM_IT_CC2) !=RESET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_CC2);
		TIM_OCStruct2.TIM_Pulse += ch2;
    if(TIM_OCStruct2.TIM_Pulse > 9999)
      TIM_OCStruct2.TIM_Pulse = 0;
    TIM_OC2Init(TIM1, &TIM_OCStruct2);
	}
  if(TIM_GetITStatus(TIM1,TIM_IT_CC3) !=RESET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_CC3);
		TIM_OCStruct3.TIM_Pulse += ch3;
    if(TIM_OCStruct3.TIM_Pulse > 9999)
      TIM_OCStruct3.TIM_Pulse = 0;
    TIM_OC3Init(TIM1, &TIM_OCStruct3);
	}
}


int main()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
	GPIO_InitTypeDef GPIO_InitStructA, GPIO_InitStructE, GPIO_InitStructB, GPIO_hall; // gpio structure
  GPIO_StructInit(&GPIO_InitStructA);
  GPIO_StructInit(&GPIO_InitStructE);
  GPIO_StructInit(&GPIO_InitStructB);
  GPIO_StructInit(&GPIO_hall);

  GPIO_hall.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_hall.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOA, &GPIO_hall);

  GPIO_InitStructA.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 ;
	GPIO_InitStructA.GPIO_Mode = GPIO_Mode_AF;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
  GPIO_Init(GPIOA, &GPIO_InitStructA);

  GPIO_InitStructE.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructE.GPIO_Mode = GPIO_Mode_AF;
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
  GPIO_Init(GPIOE, &GPIO_InitStructE);

  GPIO_InitStructB.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructB.GPIO_Mode = GPIO_Mode_AF;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM1);
  GPIO_Init(GPIOB, &GPIO_InitStructB);
       
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//timer structure
  TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);

  TIM_TimeBaseInitStruct.TIM_Period = 9999; //  == 4s
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0; // == 10,000Hz

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);


  TIM_OCStructInit(&TIM_OCStruct1);
  TIM_OCStructInit(&TIM_OCStruct2);
  TIM_OCStructInit(&TIM_OCStruct3);

  NVIC_InitTypeDef NVIC_InitStructure; // Interrupt structure
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  //pwm configuration
	TIM_OCStruct1.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCStruct1.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCStruct1.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCStruct1.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCStruct1.TIM_Pulse = 0;// 50% duty cycle  == 2s
  TIM_OC1Init(TIM1, &TIM_OCStruct1);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
        
  TIM_OCStruct2.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCStruct2.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCStruct2.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCStruct2.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCStruct2.TIM_Pulse = 0;// 50% duty cycle  == 2s
  TIM_OC2Init(TIM1, &TIM_OCStruct2);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
        
  TIM_OCStruct3.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCStruct3.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCStruct3.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCStruct3.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCStruct3.TIM_Pulse = 0;// 50% duty cycle  == 2s
  TIM_OC3Init(TIM1, &TIM_OCStruct3);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
        
  // advanced control features
  TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
  TIM_BDTRStructInit(&TIM_BDTRInitStructure);
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStructure.TIM_DeadTime = 1;
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
  TIM_CCPreloadControl(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
        
	TIM_Cmd(TIM1, ENABLE);
	TIM_ITConfig(TIM1, TIM_IT_CC1 ,ENABLE);
  TIM_ITConfig(TIM1, TIM_IT_CC2 ,ENABLE);
  TIM_ITConfig(TIM1, TIM_IT_CC3 ,ENABLE);
        
	while(1)
	{
  }
}
