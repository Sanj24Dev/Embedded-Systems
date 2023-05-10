#include "stm32f4xx.h"

void GPIO_Init()
{
	// Enable clock for port A
	RCC->AHB1ENR |= 1U << 0;

	// GPIO configurations for PA2 and PA3
	GPIOA->MODER |= 2U << 2*2;
	GPIOA->MODER |= 2U << 2*3;
	GPIOA->AFR[0] |= 7U << 4*2;
	GPIOA->AFR[0] |= 7U << 4*3;
	GPIOA->OSPEEDR |= 2U << 2*2;
	GPIOA->OSPEEDR |= 2U << 2*3;
}

void UART_Init()
{
	// Using USART2 which uses pins PA2 as TX and PA3 as RX

	// Enable clock for USART2
	RCC->APB1ENR |= 1U << 17;

	GPIO_Init();

	// M = 8 data bits, 1 start bit, n stop bit
	USART2->CR1 |= 0 << 12;

	// parity control disabled
	USART2->CR1 |= 0 << 10;

	// USART2 TX enable
	USART2->CR1 |= 1U << 3;

	// USART2 RX enable
	USART2->CR1 |= 1U << 2;

	// baud rate = 115200
	// baud rate = fClk / (8 * (2-OVER8) * USARTDIV
	// fClk=42MHz, OVER8=0 ==> USARTDIV=22.78
	// from the table in reference manual ==> USARTDIV=22.8125
	// fraction = 16*0.8125 = 13
	// mantissa = 22
	USART2->BRR |= 22 << 4;
	USART2->BRR |= 13;

	USART2->SR = 0;

	// enable USART2
	USART2->CR1 |= 1<<13;
}


void UART_Transmit(uint16_t *msg, int len)
{
	for(int i=0;i<len;i++)
	{
		// wait until dr not empty
		while(!(USART2->SR & (1<<7)));
		USART2->DR = msg[i];
		// wait for transmit complete
		while(!(USART2->SR & (1<<6)));
	}
}

void UART_Receive(uint16_t *msg, int len)
{
	for(int i=0;i<len;i++)
	{
		// wait until dr empty
		while(!(USART2->SR & (1<<5)));
		msg[i] = USART2->DR;
		// wait for transmit complete
		while(!(USART2->SR & (1<<6)));
	}
}






