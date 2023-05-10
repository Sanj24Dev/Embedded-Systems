// I2C to communicate with MPU6050

#include <stm32f4xx.h>

#define MPU_6050_ADDR	0x68
#define SMPLRT_DIV_REG	0x19
#define CONFIG_REG		0x1A
#define GYRO_CONFIG_REG	0x1B
#define PWR_MGMT_1		0x6B
#define WHO_AM_I_REG	0X75

volatile uint8_t DeviceAddr = MPU_6050_ADDR;


// initializes LEDs for debugging purposes
void LEDSetup()
{
	// setup LEDs - GPIOD 12,13,14,15
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // 1<<3
	GPIOD->MODER &= ~(0xFFU << 24);
	GPIOD->MODER |= (0x55 << 24);  // 01 for output
	GPIOD->ODR    = 0x0000;
}

// initializes pins for I2C comm
void I2C_GPIOSetup()
{
  // setup I2C - GPIOB 6, 9
  // enable I2C clock
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;  // 1<<21

  // setup I2C pins
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  GPIOB->MODER &= ~(3U << 6*2); // PB6
  GPIOB->MODER |=  (2 << 6*2); // AF
  GPIOB->OTYPER |= (1 << 6);   // open-drain
  GPIOB->MODER &= ~(3U << 9*2); // PB9
  GPIOB->MODER |=  (2 << 9*2); // AF
  GPIOB->OTYPER |= (1 << 9);   // open-drain

  // choose AF4 for I2C1 in Alternate Function registers
  GPIOB->AFR[0] |= (4 << 6*4);     // for pin 6
  GPIOB->AFR[1] |= (4 << (9-8)*4); // for pin 9
}

// initializes I2C registers
void I2C_Config()
{
  // reset and clear register
  I2C1->CR1 = I2C_CR1_SWRST;  // 1<<15
  I2C1->CR1 = 0;

  I2C1->CR2 |= (I2C_CR2_ITERREN); // enable error interrupt

  // fPCLK1 must be at least 2 Mhz for SM mode
  //        must be at least 4 Mhz for FM mode
  //        must be multiple of 10Mhz to reach 400 kHz
  // For SM Mode:
  //    Thigh = CCR * TPCLK1
  //    Tlow  = CCR * TPCLK1
  // So to generate 100 kHz SCL frequency
  // we need 1/100kz = 10us clock speed
  // Thigh and Tlow needs to be 5us each
  // Let's pick fPCLK1 = 10Mhz, TPCLK1 = 1/10Mhz = 100ns
  // Thigh = CCR * TPCLK1 => 5us = CCR * 100ns
  // CCR = 50
  I2C1->CR2 |= (10 << 0); // 10Mhz periph clock
  I2C1->CCR |= (50 << 0);
  // Maximum rise time.
  // Calculation is (maximum_rise_time / fPCLK1) + 1
  // In SM mode maximum allowed SCL rise time is 1000ns
  // For TPCLK1 = 100ns => (1000ns / 100ns) + 1= 10 + 1 = 11
  I2C1->TRISE |= (11 << 0); // program TRISE to 11 for 100khz
  // set own address to 00 - not really used in master mode
  I2C1->OAR1 |= (0x00 << 1);
}

// sends the start condition in I2C
void i2c_start() {
    I2C1->CR1 |= I2C_CR1_START;  //1<<8
    while(!(I2C1->SR1 & I2C_SR1_SB)); // waiting for start bit to set in SR1
}

// sends the stop condition in I2C
void i2c_stop() {
	// sending stop bit
    I2C1->CR1 |= I2C_CR1_STOP;  //1<<9
    while(!(I2C1->SR2 & I2C_SR2_BUSY)); // waiting for bus to stop communication
    // check
}


/**
 * This function is used to write to a register
 * 		params: regaddr - address of the register to write to
 * 				data - msg to be written
 */

void i2c_write(uint8_t regaddr, uint8_t data) {
  // start condition
  i2c_start();

  // send device addr in write mode
  // wait until address is sent
  I2C1->DR = DeviceAddr<<1 | 0x00;
  while (!(I2C1->SR1 & I2C_SR1_ADDR));  // Bit1 = 1 received address matched
  // dummy read to clear flags
  (void)I2C1->SR2; // clear addr condition

  // send reg addr
  // wait until byte transfer complete (BTF)
  I2C1->DR = regaddr;
  while (!(I2C1->SR1 & I2C_SR1_BTF));		// Bit2 = 1 when byte transfer finished

  // send data
  // wait until byte transfer complete
  I2C1->DR = data;
  while (!(I2C1->SR1 & I2C_SR1_BTF));

  // stop condition
  i2c_stop();
}


/**
 * This function is used to read a register
 * 		params: regaddr - address of the register to read from
 * 		ret value: data that is read
 */

void i2c_read(uint8_t *ret, uint8_t regaddr, int len) {
	int i;
	len = len+1;
	for(i=1;i<len;i++){
		// start condition
		i2c_start();

		// send device addr in write mode
		// wait until address is sent
		I2C1->DR = DeviceAddr<<1 | 0x00;
		while (!(I2C1->SR1 & I2C_SR1_ADDR));

		// dummy read to clear flags
		(void)I2C1->SR2; // clear addr condition

		// send reg addr
		// wait until byte transfer complete (BTF)
		I2C1->DR = regaddr;
		while (!(I2C1->SR1 & I2C_SR1_BTF));



		// restart transmission by performing repeated start
		i2c_start();

		// send device addr in read mode. LSB is 1
		// wait until address is sent
		I2C1->DR = DeviceAddr<<1 | 0x01; // read
		while (!(I2C1->SR1 & I2C_SR1_ADDR));

		// dummy read to clear flags
		(void)I2C1->SR2; // clear addr condition


		// wait until receive buffer is not empty
		while (!(I2C1->SR1 & I2C_SR1_RXNE));		// bit6 = 1 when data register not empty
		// read content
		ret[i-1] = (uint8_t)I2C1->DR;
		regaddr = regaddr + 0x01;

		// stop condition
		i2c_stop();
	 }



}

/**
 * This function handles error
 */

void I2C1_ER_IRQHandler(){
  GPIOD->ODR |= (1 << 14); // red LED
}

void MPU_Config()
{
	// power on - sending 0 for sleep bit
	i2c_write(PWR_MGMT_1, 0x00);

	// setting the gyroscope output rate = 1kHz
	i2c_write(CONFIG_REG, 0x01);

	// setting the sample rate
	i2c_write(SMPLRT_DIV_REG, 0x63);

	// setting full scale range to 500 deg/sec
	i2c_write(GYRO_CONFIG_REG, 0x08);
}

/*************************************************
			main code starts from here
*************************************************/

int main(void)
{
	LEDSetup();
	I2C_GPIOSetup();
	I2C_Config();

  // enable error interrupt from NVIC
  NVIC_SetPriority(I2C1_ER_IRQn, 1);
  NVIC_EnableIRQ(I2C1_ER_IRQn);

  I2C1->CR1 |= I2C_CR1_PE; // enable i2c

  // configure mpu6050
  MPU_Config();

  // read who am i register
  uint8_t ret;
  i2c_read(&ret, WHO_AM_I_REG, 1);

  // asking for 6 bytes of data
  uint8_t gyro_val[6];
  i2c_read(&gyro_val[0], 0x43, 6);

  uint16_t final_val[3];
  for(int i=0;i<3;i++)
  {
    final_val[i] = gyro_val[2*i]<<8 | gyro_val[2*i + 1];
  }

  while(1){
      GPIOD->ODR |= (1 << 12); // green led on completion
  }
  return 0;
}
