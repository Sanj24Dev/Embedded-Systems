#include "stm32f4xx.h"

// SPI communication between MCU and DAC
// using SPI1
// PA4 = NSS
// PA5 = CLK
// PA6 = MISO
// PA7 = MOSI
// 


// to check how to send 24-bit data 
// and how conversion happens

uint16_t converted = 0;

int main()
{
	GPIO_InitTypeDef SCK, MOSI, MISO, CS;
	SPI_InitTypeDef eeprom_spi;
	uint16_t toConvert = 10;
	
	
	MOSI.GPIO_Pin = GPIO_Pin_7; //check which gpio pin
	MOSI.GPIO_Mode = GPIO_Mode_AF;
	MOSI.GPIO_OType = GPIO_OType_PP;
	MOSI.GPIO_PuPd = GPIO_PuPd_NOPULL;
	MOSI.GPIO_Speed = GPIO_Speed_2MHz;
	
	MISO.GPIO_Pin = GPIO_Pin_6; //check which gpio pin
	MISO.GPIO_Mode = GPIO_Mode_AF;
	MISO.GPIO_OType = GPIO_OType_PP;
	MISO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	MISO.GPIO_Speed = GPIO_Speed_2MHz;

	CS.GPIO_Pin = GPIO_Pin_4; //check which gpio pin
	CS.GPIO_Mode = GPIO_Mode_OUT;
	CS.GPIO_OType = GPIO_OType_PP;
	CS.GPIO_PuPd = GPIO_PuPd_NOPULL;
	CS.GPIO_Speed = GPIO_Speed_2MHz;
	
	SCK.GPIO_Pin = GPIO_Pin_5; //check which gpio pin
	SCK.GPIO_Mode = GPIO_Mode_AF;
	SCK.GPIO_OType = GPIO_OType_PP;
	SCK.GPIO_PuPd = GPIO_PuPd_NOPULL;
	SCK.GPIO_Speed = GPIO_Speed_2MHz;
	
	eeprom_spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	eeprom_spi.SPI_Mode = SPI_Mode_Master;
	eeprom_spi.SPI_DataSize = SPI_DataSize_8b;
	eeprom_spi.SPI_CPOL = SPI_CPOL_High;
	eeprom_spi.SPI_CPHA = SPI_CPHA_2Edge;
	eeprom_spi.SPI_NSS = SPI_NSS_Hard;
	eeprom_spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	
	
	RCC_AHB1PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	GPIO_Init(GPIOA, &MOSI); 
	GPIO_Init(GPIOA, &MISO);
	GPIO_Init(GPIOA, &CS);
	GPIO_Init(GPIOA, &SCK);
	
	SPI_Init(SPI1, &eeprom_spi);
	
	SPI_I2S_SendData(SPI1,toConvert);
	converted = SPI_I2S_ReceiveData(SPI1);
	
	while(1);
	
	
	
}
