/* _____  _____  __  __             _____  _____ 
  / ____||_   _||  \/  |    /\     / ____||_   _|
 | |  __   | |  | \  / |   /  \   | (___    | |  
 | | |_ |  | |  | |\/| |  / /\ \   \___ \   | |  
 | |__| | _| |_ | |  | | / ____ \  ____) | _| |_ 
  \_____||_____||_|  |_|/_/    \_\|_____/ |_____|
  (c) 2017 GIMASI SA                                               

 * hw_spi2.c
 *  Second User SPI used as slave
 *  Created on: Feb 21, 2017
 *      Author: Massimo Santoli
 */

/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Bleeper board SPI driver implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
 /*******************************************************************************
  * @file    hw_spi.c
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    01-January-2017
  * @brief   manages the SPI interface
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "hw.h"
#include "utilities.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;
/* Private function prototypes -----------------------------------------------*/


/* Exported functions ---------------------------------------------------------*/

/*!
 * @brief Initializes the SPI object and MCU peripheral
 *
 * @param [IN] none
 */
void HW_SPI2_Init( void )
{

	 hspi2.Instance = SPI2;
	 hspi2.Init.Mode = SPI_MODE_SLAVE;
	 hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	 hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	 hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	 hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	 hspi2.Init.NSS = SPI_NSS_HARD_INPUT;
	 hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	 hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	 hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	 hspi2.Init.CRCPolynomial = 7;

	 if (HAL_SPI_Init(&hspi2) != HAL_OK)
	 {
		 Error_Handler();
	 }

	 /*##-2- Configure the SPI GPIOs */
	 HW_SPI2_IoInit(  );
}

/*!
 * @brief De-initializes the SPI object and MCU peripheral
 *
 * @param [IN] none
 */
void HW_SPI2_DeInit( void )
{

  HAL_SPI_DeInit( &hspi2);

    /*##-1- Reset peripherals ####*/
  __HAL_RCC_SPI2_FORCE_RESET();
  __HAL_RCC_SPI2_RELEASE_RESET();
  /*##-2- Configure the SPI GPIOs */
  HW_SPI2_IoDeInit( );
}

void HW_SPI2_IoInit( void )
{
  GPIO_InitTypeDef initStruct={0};

  __HAL_RCC_SPI2_CLK_ENABLE();

  initStruct.Pin = USER_SPI_MOSI|USER_SPI_MISO|USER_SPI_SCK|USER_SPI_NSS;
  initStruct.Mode = GPIO_MODE_AF_PP;
  initStruct.Pull = GPIO_NOPULL;
  initStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  initStruct.Alternate = GPIO_AF0_SPI2;
  HAL_GPIO_Init(USER_SPI_PORT, &initStruct);
  /* Peripheral interrupt init */
  HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SPI2_IRQn);


}


void HW_SPI2_IoDeInit( void )
{
  GPIO_InitTypeDef initStruct={0};

  initStruct.Mode =GPIO_MODE_OUTPUT_PP;

  initStruct.Pull =GPIO_PULLDOWN  ;
  HW_GPIO_Init ( USER_SPI_PORT, USER_SPI_MOSI, &initStruct );
  HW_GPIO_Write( USER_SPI_PORT, USER_SPI_MOSI, 0 );

  initStruct.Pull =GPIO_PULLDOWN;
  HW_GPIO_Init ( USER_SPI_PORT, USER_SPI_MISO, &initStruct );
  HW_GPIO_Write( USER_SPI_PORT, USER_SPI_MISO, 0 );

  initStruct.Pull =GPIO_PULLDOWN  ;
  HW_GPIO_Init ( USER_SPI_PORT, USER_SPI_SCK, &initStruct );
  HW_GPIO_Write(  USER_SPI_PORT, USER_SPI_SCK, 0 );

  initStruct.Pull = GPIO_PULLUP;
  HW_GPIO_Init ( USER_SPI_PORT, USER_SPI_NSS , &initStruct );
  HW_GPIO_Write( USER_SPI_PORT, USER_SPI_NSS , 1 );
}

/*!
 * @brief Sends outData and receives inData
 *
 * @param [IN] outData Byte to be sent
 * @retval inData      Received byte.
 */
uint16_t HW_SPI2_InOut( uint16_t txData )
{
  uint16_t rxData ;

  HAL_SPI_TransmitReceive( &hspi2, ( uint8_t * ) &txData, ( uint8_t* ) &rxData, 1, HAL_MAX_DELAY);

  return rxData;
}

/* Private functions ---------------------------------------------------------*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

