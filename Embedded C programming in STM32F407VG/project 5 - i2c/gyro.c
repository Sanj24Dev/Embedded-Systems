#include "gyro.h"

uint8_t RxBuffer = 0, TxBuffer = 0;
I2C_InitTypeDef pCPAL_I2C_Struct;
CPAL_TransferTypeDef Rx_Struct, Tx_Struct;

uint32_t BSP_Gyroscope_Init()
{
  I2C_StructInit(&pCPAL_I2C_Struct);
  pCPAL_I2C_Struct.I2C_ClockSpeed = 400000;
  pCPAL_I2C_Struct.I2C_Mode = I2C_Mode_I2C;
  pCPAL_I2C_Struct.I2C_DutyCycle = I2C_DutyCycle_2;
  pCPAL_I2C_Struct.I2C_OwnAddress1 = 0;
  pCPAL_I2C_Struct.I2C_Ack = I2C_Ack_Enable;
  pCPAL_I2C_Struct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init(I2C1, &pCPAL_I2C_Struct);
  
  
  Rx_Struct.pbBuffer = &RxBuffer ;
  Rx_Struct.wAddr1 = 0x68<<1;		
  Rx_Struct.wNumData = 1;	
  
  Tx_Struct.pbBuffer = &TxBuffer;
  Tx_Struct.wAddr1 = 0x68<<1;		
  Tx_Struct.wNumData = 1;
  
  I2C1_DevStructure.CPAL_Dev = CPAL_I2C1;
  I2C1_DevStructure.CPAL_State = CPAL_STATE_READY;
  I2C1_DevStructure.CPAL_Direction = CPAL_DIRECTION_TXRX;
  I2C1_DevStructure.CPAL_Mode = CPAL_MODE_MASTER;
  I2C1_DevStructure.CPAL_ProgModel = CPAL_PROGMODEL_INTERRUPT;
  I2C1_DevStructure.pCPAL_TransferRx = &Rx_Struct;
  I2C1_DevStructure.pCPAL_TransferTx = &Tx_Struct;
  I2C1_DevStructure.pCPAL_I2C_Struct = &pCPAL_I2C_Struct;
  
  if( CPAL_I2C_Init(&I2C1_DevStructure) == CPAL_FAIL)
    return 0;
  return 1;
}


void BSP_Gyroscope_Read_WhoAmI(uint8_t *buff)
{
  Rx_Struct.wAddr2 = 0x75;
  CPAL_I2C_Read(&I2C1_DevStructure);
  while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY);
  
  *buff = RxBuffer;
}

void BSP_Gyroscope_Read_Register(uint8_t *buff, uint8_t addr)
{
  Rx_Struct.wAddr2 = addr;
  CPAL_I2C_Read(&I2C1_DevStructure);
  while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY);
  
  *buff = RxBuffer;
}


float  factoryTrim[6];
float destination[6];
uint8_t BSP_Gyroscope_SelfTest()
{
    uint8_t rawData[4];
    float selfTest[6];
    int i;

    uint32_t TIMER_Simulator_Var;

    Tx_Struct.wAddr2 = 0x1B;
    Tx_Struct.wNumData = 1;
    TxBuffer=0xE0;
    if (CPAL_I2C_Write(&I2C1_DevStructure) != CPAL_PASS)
    { /* I2C bus or peripheral is not able to start communication: Error management */}
    /* Wait the end of transfer */

    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }

    Tx_Struct.wAddr2 = 0x1C;
    Tx_Struct.wNumData = 1;
    TxBuffer=0xF0;
    if (CPAL_I2C_Write(&I2C1_DevStructure) != CPAL_PASS)
    { /* I2C bus or peripheral is not able to start communication: Error management */  }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }


    Rx_Struct.wAddr2 = SELF_TEST_X;
    Rx_Struct.wNumData = 1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    rawData[0] = RxBuffer;

    
    
    Rx_Struct.wAddr2 = SELF_TEST_Y;
    Rx_Struct.wNumData = 1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    rawData[1] = RxBuffer;

    
    Rx_Struct.wAddr2 = SELF_TEST_Z;
    Rx_Struct.wNumData = 1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    rawData[2] = RxBuffer;


    Rx_Struct.wAddr2 =0x10;
    Rx_Struct.wNumData = 1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    rawData[3] = RxBuffer;


//  rawData[0]=*BSP_Gyroscope_ReadRegister(I2C1_DevStructure,SELF_TEST_X);
//  rawData[1]=*BSP_Gyroscope_ReadRegister(I2C1_DevStructure,SELF_TEST_Y);
//  rawData[2]=*BSP_Gyroscope_ReadRegister(I2C1_DevStructure,SELF_TEST_Z);
//  rawData[3]=*BSP_Gyroscope_ReadRegister(I2C1_DevStructure,0x10);

    // Extract the acceleration test results first
    selfTest[0] = (rawData[0] >> 3) | (rawData[3] & 0x30) >> 4 ; // XA_TEST result is a five-bit unsigned integer
    selfTest[1] = (rawData[1] >> 3) | (rawData[3] & 0x0C) >> 2 ; // YA_TEST result is a five-bit unsigned integer
    selfTest[2] = (rawData[2] >> 3) | (rawData[3] & 0x03) >> 0 ; // ZA_TEST result is a five-bit unsigned integer
    // Extract the gyration test results first
    selfTest[3] = rawData[0]  & 0x1F ; // XG_TEST result is a five-bit unsigned integer
    selfTest[4] = rawData[1]  & 0x1F ; // YG_TEST result is a five-bit unsigned integer
    selfTest[5] = rawData[2]  & 0x1F ; // ZG_TEST result is a five-bit unsigned integer

    // Process results to allow final comparison with factory set values
    factoryTrim[0] = (int)(4096.0*0.34)*(int)(pow( (0.92/0.34), (((float)selfTest[0] - 1.0)/30.0)));  // FT[Xa] factory trim calculation
    factoryTrim[1] = (int)(4096.0*0.34)*(int)(pow( (0.92/0.34), (((float)selfTest[1] - 1.0)/30.0)));  // FT[Ya] factory trim calculation
    factoryTrim[2] = (int)(4096.0*0.34)*(int)(pow( (0.92/0.34), (((float)selfTest[2] - 1.0)/30.0)));  // FT[Za] factory trim calculation
    factoryTrim[3] =  (int)( 25.0*131.0)*(int)(pow( 1.046, ((float)selfTest[3] - 1.0) ));              // FT[Xg] factory trim calculation
    factoryTrim[4] =  (int)(-25.0*131.0)*(int)(pow( 1.046, ((float)selfTest[4] - 1.0) ));              // FT[Yg] factory trim calculation
    factoryTrim[5] =  (int)( 25.0*131.0)*(int)(pow( 1.046, ((float)selfTest[5] - 1.0) ));              // FT[Zg] factory trim calculation

    for (i = 0; i < 6; i++)
        destination[i] = 100.0 + (100.0*((float)selfTest[i] - factoryTrim[i])/factoryTrim[i]); // Report percent differences

    if(destination[0]<1 && destination[1]<1 && destination[2] < 1 && destination[3] < 1 && destination[4] < 1 && destination[5] < 1)
        return SUCCESS;
    return      ERROR;
}



int16_t acc_x,acc_y,acc_z,gyr_x,gyr_y,gyr_z,temp;
float b,c,d,e;
uint8_t addr, ready;


uint8_t fifo[2];
uint8_t rec[256];

uint8_t BSP_Gyroscope_GetValues(BSP_GyrFetchTypeDef* BSP_GyrFetchStructure)
{
    uint32_t TIMER_Simulator_Var;

    for(int i=0; i<256; i++)
    {
        rec[i]=0;
    }
    uint16_t info[14];
    ready = 0;
    

    /*******/
    Tx_Struct.wAddr2 = 0x6B;
    Tx_Struct.wNumData = 1;
    TxBuffer=0x00;
    if (CPAL_I2C_Write(&I2C1_DevStructure) != CPAL_PASS)
    { /* I2C bus or peripheral is not able to start communication: Error management */  }
    /* Wait the end of transfer */

    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }

    Tx_Struct.wAddr2 = 0x1C;
    Tx_Struct.wNumData = 1;
    TxBuffer=0x00;
    if (CPAL_I2C_Write(&I2C1_DevStructure) != CPAL_PASS)
    { /* I2C bus or peripheral is not able to start communication: Error management */  }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    /*************/
    Tx_Struct.wAddr2 = 0x1B;
    Tx_Struct.wNumData = 1;
    TxBuffer=0x00;
    if (CPAL_I2C_Write(&I2C1_DevStructure) != CPAL_PASS)
    { /* I2C bus or peripheral is not able to start communication: Error management */  }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }



    Rx_Struct.wAddr2 = 0x41;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    temp=temp|RxBuffer;
    temp=temp<<8;

    Rx_Struct.wAddr2 = 0x42;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    temp=temp | RxBuffer;


    Rx_Struct.wAddr2 = 0x43;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    gyr_x=gyr_x|RxBuffer;
    gyr_x=gyr_x<<8;

    Rx_Struct.wAddr2 = 0x44;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    gyr_x=gyr_x | RxBuffer;

    Rx_Struct.wAddr2 = 0x45;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    gyr_y=gyr_y|RxBuffer;
    gyr_y=gyr_y<<8;

    Rx_Struct.wAddr2 = 0x46;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    gyr_y= gyr_y|RxBuffer;


    Rx_Struct.wAddr2 = 0x47;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    gyr_z=gyr_z|RxBuffer;
    gyr_z=gyr_z<<8;

    Rx_Struct.wAddr2 = 0x48;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    gyr_z =gyr_z|RxBuffer;

    Rx_Struct.wAddr2 = 0x3B;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    acc_x=acc_x|RxBuffer;
    acc_x=acc_x<<8;

    Rx_Struct.wAddr2 = 0x3C;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    acc_x = acc_x|RxBuffer;

    Rx_Struct.wAddr2 = 0x3D;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    acc_y=acc_y|RxBuffer;
    acc_y=acc_y<<8;

    Rx_Struct.wAddr2 = 0x3E;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    acc_y = acc_y|RxBuffer;

    Rx_Struct.wAddr2 = 0x3F;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    acc_z= acc_z|RxBuffer;
    acc_z=acc_z<<8;

    Rx_Struct.wAddr2 = 0x40;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    acc_z= acc_z|RxBuffer;

    Rx_Struct.wAddr2 = 0x68;
    Rx_Struct.wNumData =1;
    if (CPAL_I2C_Read(&I2C1_DevStructure) != CPAL_PASS)
    {
        return ERROR;
    }
    /* Wait the end of transfer */
    TIMER_Simulator_Var = 0;
    while(I2C1_DevStructure.CPAL_State != CPAL_STATE_READY  && (++TIMER_Simulator_Var < TIMER_TIMEOUT_I2C));
    if(TIMER_Simulator_Var == TIMER_TIMEOUT_I2C)
    {
        return ERROR;
    }
    addr = RxBuffer;





    BSP_GyrFetchStructure->BSP_Acc_x=((float)acc_x)/16384.0;
    BSP_GyrFetchStructure->BSP_Acc_y=((float)acc_y)/16384.0;
    BSP_GyrFetchStructure->BSP_Acc_z=((float)acc_z)/16384.0;
    BSP_GyrFetchStructure->BSP_Temp=temp/340.0+ 36.53;
    BSP_GyrFetchStructure->BSP_Gyr_x=gyr_x/131.0;
    BSP_GyrFetchStructure->BSP_Gyr_y=gyr_y/131.0;
    BSP_GyrFetchStructure->BSP_Gyr_z=gyr_z/131.0;


    return SUCCESS;
}