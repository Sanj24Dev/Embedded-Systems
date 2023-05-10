/**
	PD12 pin is for blue LED and this code toggles the pin
	working code
 */

#include <stdint.h>
#include <stm32f4xx.h>


int main(void)
{
    RCC->AHB1ENR |= 0x00000008;

    GPIOD->MODER &= 0x3FFFFFFF;
    GPIOD->MODER |= 0x40000000;

//    GPIOD->OTYPER &= 0xFFFF7FFF;
//    GPIOD->OTYPER |= 0x00008000;
//
//    GPIOD->OSPEEDR &= 0x3FFFFFFF;
//    GPIOD->OSPEEDR |= 0x80000000;
//
//    GPIOD->PUPDR &= 0x3FFFFFFF;
//    GPIOD->PUPDR |= 0x40000000;


	while(1)
	{
		GPIOD->ODR = 0x8000;
		for(int i=0;i<1000000;i++);
		GPIOD->ODR = 0x0000;
		for(int i=0;i<1000000;i++);
	}
}
