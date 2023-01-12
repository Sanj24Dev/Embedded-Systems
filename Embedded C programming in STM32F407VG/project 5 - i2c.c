/* 
  This program reads the WHO_AM_I register in the gyroscope(MPU6050)
*/

#include "stm32f4xx.h"
#include "main.h"
#include "cpal.h"


uint8_t RxBuffer = 0;


int main()
{
  I2C_InitTypeDef pCPAL_I2C_Struct;
  pCPAL_I2C_Struct.I2C_ClockSpeed = 400000;
  pCPAL_I2C_Struct.I2C_Mode = I2C_Mode_I2C;
  pCPAL_I2C_Struct.I2C_DutyCycle = I2C_DutyCycle_2;
  pCPAL_I2C_Struct.I2C_OwnAddress1 = 0;
  pCPAL_I2C_Struct.I2C_Ack = I2C_Ack_Enable;
  pCPAL_I2C_Struct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init(&pCPAL_I2C_Struct);
  
  /*
  // Transfer structure
  CPAL_TransferTypeDef Tx_Init;
  Tx_Init.pbBuffer = 0x00;	//!<The address of the buffer from/to which the transfer should start 
  Tx_Init.wAddr1 = 0xD0;	//!<Contains the Target device Address (optional)
  Tx_Init.wAddr2 = 0x6B;	//!<Contains the Register/Physical Address into the device (optional) 
  Tx_Init.wNumData = 1 ; //!<Number of data to be transferred for the current transaction  
  */
  
  // Receiving structure
  CPAL_TransferTypeDef Rx_Init;
  Rx_Init.pbBuffer = &RxBuffer ;	/*!<The address of the buffer from/to which the transfer should start */
  Rx_Init.wAddr1 = 0x68<<1;	/*!<Contains the Target device Address (optional)*/
  Rx_Init.wAddr2 = 0x75;	/*!<Contains the Register/Physical Address into the device (optional) */
  Rx_Init.wNumData = 1;	/*!<Number of data to be transferred for the current transaction */ 
  
  
  I2C1_DevStructure.CPAL_Dev = CPAL_I2C1;
  I2C1_DevStructure.CPAL_State = CPAL_STATE_READY;
  I2C1_DevStructure.CPAL_Direction = CPAL_DIRECTION_TXRX;
  I2C1_DevStructure.CPAL_Mode = CPAL_MODE_MASTER;
  I2C1_DevStructure.CPAL_ProgModel = CPAL_PROGMODEL_INTERRUPT;
  // I2C1_DevStructure.pCPAL_TransferTx = &Tx_Init;
  I2C1_DevStructure.pCPAL_TransferRx = &Rx_Init;
  I2C1_DevStructure.pCPAL_I2C_Struct = &pCPAL_I2C_Struct;
  CPAL_I2C_Init(&I2C1_DevStructure);
  
  // CPAL_I2C_Write(&I2C1_DevStructure);
  CPAL_I2C_Read(&I2C1_DevStructure);
  // waiting for the end of transfer
  while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY);
  

  while(1); 
  
}
