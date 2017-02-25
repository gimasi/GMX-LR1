/* _____  _____  __  __             _____  _____
  / ____||_   _||  \/  |    /\     / ____||_   _|
 | |  __   | |  | \  / |   /  \   | (___    | |
 | | |_ |  | |  | |\/| |  / /\ \   \___ \   | |
 | |__| | _| |_ | |  | | / ____ \  ____) | _| |_
  \_____||_____||_|  |_|/_/    \_\|_____/ |_____|
  (c) 2017 GIMASI SA

 * hw_i2c.h
 *
 *  Created on: Feb 19, 2017
 *      Author: Massimo Santoli
 */

#include "hw.h"
#include "utilities.h"


static I2C_HandleTypeDef hi2c1;


void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
 /* NOTE : This function Should not be modified, when the callback is needed,
           the HAL_I2C_MspInit could be implemented in the user file
  */

	GPIO_InitTypeDef GPIO_InitStruct;
	     if(hi2c->Instance==I2C1)
	     {
	         GPIO_InitStruct.Pin = I2C_PIN_SDA|I2C_PIN_SCL;
	         GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	         GPIO_InitStruct.Pull = GPIO_PULLUP;
	         GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	         GPIO_InitStruct.Alternate = I2C1_AF;
	         HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	         /* Peripheral clock enable */
	         __I2C1_CLK_ENABLE();
	     }

}


void HW_I2C_Init( void ) {


	I2C_HandleTypeDef empty={0};

	  hi2c1=empty;

	  hi2c1.Instance = I2C1;
	  hi2c1.Init.Timing = 0x00000708;
	  hi2c1.Init.OwnAddress1 = 0;
	  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c1.Init.OwnAddress2 = 0;
	  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /**Configure Analogue filter
	     */
	   if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	   {
	     Error_Handler();
	   }

}


void HW_I2C_Reset( void ) {

	 __HAL_I2C_DISABLE(&hi2c1);
	 __HAL_I2C_ENABLE(&hi2c1);

}

HAL_StatusTypeDef  HW_I2C_ReadRegister(uint8_t devAddr,uint8_t devReg, uint8_t NbByte , uint8_t *pBuffer ){

	uint8_t wBuffer[1];
	uint8_t wSize = 0;

	wBuffer[0] = devReg;
	wSize = 1;

	if ( HAL_I2C_Master_Transmit(&hi2c1, devAddr, wBuffer, wSize, 1000) == HAL_OK ) {
		if( HAL_I2C_Master_Receive(&hi2c1, devAddr, pBuffer, NbByte, 1000) == HAL_OK ) {
			return HAL_OK;
		}
	}

	return HAL_ERROR;
}


HAL_StatusTypeDef  HW_I2C_WriteRegister(uint8_t devAddr,uint8_t devReg ,uint8_t data) {

	uint8_t wBuffer[2];

	wBuffer[0] = devReg;
	wBuffer[1] = data;

	if ( HAL_I2C_Master_Transmit(&hi2c1, devAddr, wBuffer, 2, 1000) == HAL_OK ) {
		return HAL_OK;
	}

	return HAL_ERROR;
}


HAL_StatusTypeDef  HW_I2C_WriteRegisterMulti(uint8_t devAddr,uint8_t devReg ,uint8_t NbByte , uint8_t *pBuffer) {

	uint8_t wBuffer[64];
	uint8_t wSize = 0;


	if (NbByte > 63)
		return HAL_ERROR;

	wBuffer[0] = devReg;
	for (wSize = 1; wSize <= NbByte; wSize ++)
		wBuffer[wSize] = pBuffer[wSize-1];


	if ( HAL_I2C_Master_Transmit(&hi2c1, devAddr, wBuffer, wSize, 1000) == HAL_OK ) {
		return HAL_OK;
	}

	return HAL_ERROR;
}

HAL_StatusTypeDef  HW_I2C_ReadData(uint8_t addr,uint8_t NbByte , uint8_t *pBuffer ){

	return( HAL_I2C_Master_Receive(&hi2c1, addr, pBuffer, NbByte, 2000));

}


HAL_StatusTypeDef HW_I2C_WriteData ( uint8_t devAddr,uint8_t NbByte , uint8_t *pBuffer ) {


	return( HAL_I2C_Master_Transmit(&hi2c1, devAddr, pBuffer, NbByte, 2000) );
}



HAL_StatusTypeDef HW_I2C_DeviceReady( uint16_t DevAddress, uint32_t Trials, uint32_t Timeout ) {

	return( HAL_I2C_IsDeviceReady( &hi2c1, DevAddress, Trials, Timeout) );

}


void HW_I2C_ClockStretch( void ) {

	HAL_Delay(5);
	RCC->APB1RSTR |= 0x00200000; // Reset i2c peripheral
	RCC->APB1RSTR &= 0xFFDFFFFF;

	HW_I2C_Init();

}


void HW_I2C_DeInit( void ) {

	HAL_I2C_DeInit( &hi2c1);
}
