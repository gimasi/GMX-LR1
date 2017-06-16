/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: SX1276 driver specific target board functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
 /*******************************************************************************
  * @file    mlm32l07x01.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    15-November-2016
  * @brief   driver LoRa module murata cmwx1zzabz-078
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>
#include "radio.h"
#include "sx1276.h"
#include "mlm32l07x01.h"
#include "delay.h"
#include "timeServer.h"
#include "hw.h"

#define IRQ_HIGH_PRIORITY  0

#ifdef TCXO_PRESENT
/* Delay in ms between radio set in sleep mode and TCXO off*/
#define TCXO_OFF_DELAY 2 
/*!
 * callback to stop TCXO on timer timeout
 */
static TimerEvent_t TcxoStopTimer;

static void OnTcxoStopTimerEvent( void ); 
#endif

/*!
 * Flag used to set the RF switch control pins in low power mode when the radio is not active.
 */
static bool RadioIsActive = false;

/*!
 * Radio driver structure initialization
 */
const struct Radio_s Radio =
{
  SX1276IoInit,
  SX1276IoDeInit,
  SX1276Init,
  SX1276GetStatus,
  SX1276SetModem,
  SX1276SetChannel,
  SX1276IsChannelFree,
  SX1276Random,
  SX1276SetRxConfig,
  SX1276SetTxConfig,
  SX1276CheckRfFrequency,
  SX1276GetTimeOnAir,
  SX1276Send,
  SX1276SetSleep,
  SX1276SetStby, 
  SX1276SetRx,
  SX1276StartCad,
  SX1276ReadRssi,
  SX1276Write,
  SX1276Read,
  SX1276WriteBuffer,
  SX1276ReadBuffer,
  SX1276SetSyncWord,
  SX1276SetMaxPayloadLength
};


void SX1276IoInit( void )
{
  GPIO_InitTypeDef initStruct={0};
  
  initStruct.Mode =GPIO_MODE_IT_RISING;
  initStruct.Pull = GPIO_PULLUP;
  initStruct.Speed = GPIO_SPEED_HIGH;

  HW_GPIO_Init( RADIO_DIO_0_PORT, RADIO_DIO_0_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_1_PORT, RADIO_DIO_1_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_2_PORT, RADIO_DIO_2_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_3_PORT, RADIO_DIO_3_PIN, &initStruct );

#ifdef TCXO_PRESENT
  initStruct.Mode =GPIO_MODE_OUTPUT_PP;
  initStruct.Pull = GPIO_NOPULL;  
  HW_GPIO_Init( RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN, &initStruct );
#endif
}

void SX1276IoIrqInit( DioIrqHandler **irqHandlers )
{
  HW_GPIO_SetIrq( RADIO_DIO_0_PORT, RADIO_DIO_0_PIN, IRQ_HIGH_PRIORITY, irqHandlers[0] );
  HW_GPIO_SetIrq( RADIO_DIO_1_PORT, RADIO_DIO_1_PIN, IRQ_HIGH_PRIORITY, irqHandlers[1] );
  HW_GPIO_SetIrq( RADIO_DIO_2_PORT, RADIO_DIO_2_PIN, IRQ_HIGH_PRIORITY, irqHandlers[2] );
  HW_GPIO_SetIrq( RADIO_DIO_3_PORT, RADIO_DIO_3_PIN, IRQ_HIGH_PRIORITY, irqHandlers[3] );
}


void SX1276IoDeInit( void )
{
  GPIO_InitTypeDef initStruct={0};

  initStruct.Mode = GPIO_MODE_IT_RISING ; //GPIO_MODE_ANALOG;
  initStruct.Pull = GPIO_PULLDOWN;
  
  HW_GPIO_Init( RADIO_DIO_0_PORT, RADIO_DIO_0_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_1_PORT, RADIO_DIO_1_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_2_PORT, RADIO_DIO_2_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_3_PORT, RADIO_DIO_3_PIN, &initStruct );
}

uint8_t SX1276GetPaSelect( uint32_t channel )
{
    return RF_PACONFIG_PASELECT_RFO;
}

static void SX1276AntSwInit( void )
{
  GPIO_InitTypeDef initStruct={0};

  initStruct.Mode =GPIO_MODE_OUTPUT_PP;
  initStruct.Pull = GPIO_NOPULL; //GPIO_PULLUP;
  initStruct.Speed = GPIO_SPEED_HIGH;
  
  HW_GPIO_Init( RADIO_ANT_SWITCH_PORT_RX, RADIO_ANT_SWITCH_PIN_RX, &initStruct  ); 
  HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_RX, RADIO_ANT_SWITCH_PIN_RX, 0);
  
  HW_GPIO_Init( RADIO_ANT_SWITCH_PORT_TX_BOOST, RADIO_ANT_SWITCH_PIN_TX_BOOST, &initStruct  ); 
  HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_TX_BOOST, RADIO_ANT_SWITCH_PIN_TX_BOOST, 0);
  
  HW_GPIO_Init( RADIO_ANT_SWITCH_PORT_TX_RFO, RADIO_ANT_SWITCH_PIN_TX_RFO, &initStruct  ); 
  HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_TX_RFO, RADIO_ANT_SWITCH_PIN_TX_RFO, 0);
}

static void SX1276AntSwDeInit( void )
{
  GPIO_InitTypeDef initStruct={0};

  initStruct.Mode = GPIO_MODE_ANALOG ;
  
  initStruct.Pull = GPIO_NOPULL;
  initStruct.Speed = GPIO_SPEED_HIGH;

  HW_GPIO_Init( RADIO_ANT_SWITCH_PORT_RX, RADIO_ANT_SWITCH_PIN_RX, &initStruct  ); 
  HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_RX, RADIO_ANT_SWITCH_PIN_RX, 0);
  
  HW_GPIO_Init( RADIO_ANT_SWITCH_PORT_TX_BOOST, RADIO_ANT_SWITCH_PIN_TX_BOOST, &initStruct  ); 
  HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_TX_BOOST, RADIO_ANT_SWITCH_PIN_TX_BOOST, 0);
  
  HW_GPIO_Init( RADIO_ANT_SWITCH_PORT_TX_RFO, RADIO_ANT_SWITCH_PIN_TX_RFO, &initStruct  ); 
  HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_TX_RFO, RADIO_ANT_SWITCH_PIN_TX_RFO, 0);
}



void SX1276SetAntSwLowPower( bool status )
{
  if( RadioIsActive != status )
  {
    RadioIsActive = status;
    
    if( status == false )
    {
	 #ifdef TCXO_PRESENT
      TimerStop( &TcxoStopTimer );
      
      MLM_TCXO_ON();  //TCXO ON
      
      DelayMs( BOARD_WAKEUP_TIME ); //start up time of TCXO
	#endif
      SX1276AntSwInit( );
    }
    else 
    {
      SX1276AntSwDeInit( );
	#ifdef TCXO_PRESENT
      TimerInit( &TcxoStopTimer, OnTcxoStopTimerEvent );
      
      TimerSetValue( &TcxoStopTimer, TCXO_OFF_DELAY);
      
      TimerStart( &TcxoStopTimer );
	#endif
    }
  }
}

#ifdef TCXO_PRESENT
static void OnTcxoStopTimerEvent( void )
{
  TimerStop( &TcxoStopTimer );

  MLM_TCXO_OFF();  //TCXO OFF
}
#endif

void SX1276SetAntSw( uint8_t rxTx )
{
  
  SX1276.RxTx = rxTx;

  if( rxTx != 0 ) // 1: TX, 0: RX
  {
    uint8_t paConfig =  SX1276Read( REG_PACONFIG );

    if( ( paConfig & RF_PACONFIG_PASELECT_PABOOST ) == RF_PACONFIG_PASELECT_PABOOST )
    {
      HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_TX_BOOST, RADIO_ANT_SWITCH_PIN_TX_BOOST, 1 );
    }
    else
    {
      HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_TX_RFO, RADIO_ANT_SWITCH_PIN_TX_RFO, 1 );
    }
  }
  else
  {
    HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_RX, RADIO_ANT_SWITCH_PIN_RX, 1 );
  }
}

bool SX1276CheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported
    return true;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
