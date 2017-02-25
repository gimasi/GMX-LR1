/* _____  _____  __  __             _____  _____ 
  / ____||_   _||  \/  |    /\     / ____||_   _|
 | |  __   | |  | \  / |   /  \   | (___    | |  
 | | |_ |  | |  | |\/| |  / /\ \   \___ \   | |  
 | |__| | _| |_ | |  | | / ____ \  ____) | _| |_ 
  \_____||_____||_|  |_|/_/    \_\|_____/ |_____|
  (c) 2017 GIMASI SA                                               

 * application.c
 *
 *  Created on: Feb 19, 2017
 *      Author: Massimo Santoli
 *      Version: 1.0
 */

#include "hw.h"
#include "lora.h"
#include "timeServer.h"

#include "gmx_led.h"
#include "application.h"


static Application_t _app;

static bool network_joined = false;


static void OnRejoinTimerEvent( void );
static void OnTxTimerEvent( void );

static TimerEvent_t ReJoinTimer;
static TimerEvent_t TxTimer;

static void OnRejoinTimerEvent( void ){
	_app.state |= APPLICATION_REJOIN_NETWORK;
}

static void OnTxTimerEvent( void ){
	_app.state |= APPLICATION_TX_PACKET;
	GMX_Led(LED_2,LED_FLASH_2);
}


/* User Application Init */
void Application_Init()
{


	/* setup HW */
	GMX_LedInit();
	GMX_LedFlashHW(LED_1, FLASH_FAST_MS, 2);
	GMX_LedFlashHW(LED_2, FLASH_FAST_MS, 2);
	GMX_LedFlashHW(LED_3, FLASH_FAST_MS, 2);



	/* Set LoRaWAN Params */
	/* You have to specify the DevEUI in the Commissioning.h file */
	/* put your keys here or read them from EEPROM */
	 lora_configuration_t my_conf = {
			.AppEui = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
			.AppKey = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C }
	};


	lora_config_otaa_set(ENABLE);
	lora_config_appeui_set(my_conf.AppEui);
	lora_config_appkey_set(my_conf.AppKey);


	/* Join */
	lora_join();
	/* run LORA FSM to start the join */
	lora_fsm();

	/* decide if Join is blocking or not */
	//while(!network_joined){}

	TimerInit( &ReJoinTimer, OnRejoinTimerEvent );
	TimerSetValue( &ReJoinTimer, 3600000 );

	TimerInit( &TxTimer, OnTxTimerEvent );
	TimerSetValue( &TxTimer, 60000 );

	_app.state = APPLICATION_IDLE;
}



/* User Application Main Loop */
void Application_Loop()
{

	if ( _app.state & APPLICATION_REJOIN_NETWORK )
	{

		LoRaMacReset();

		lora_join();
		/* run LORA FSM to start the join */
		lora_fsm();

		_app.state &= ~APPLICATION_REJOIN_NETWORK;
	}


	if (_app.state & APPLICATION_NETWORK_JOINED)
	{
		TimerStart( &ReJoinTimer );
		TimerStart( &TxTimer );

		_app.state &= ~APPLICATION_NETWORK_JOINED;

	}

	if (_app.state & APPLICATION_TX_PACKET)
	{
		lora_send("1:010101",8, false);
		TimerStart( &TxTimer );

		_app.state &= ~APPLICATION_TX_PACKET;
	}

}


ApplicationState_t Application_getApplicationState() {
	return _app.state;
}

/* Callbacks from LoRa Stack */

/* Application Level Lora TX */
void Application_LoraTxData( lora_AppData_t *AppData, FunctionalState* IsTxConfirmed)
{
  /* USER CODE BEGIN 3 */
  AppData->Port = lora_config_application_port_get();
  *IsTxConfirmed =  lora_config_reqack_get();
  /* USER CODE END 3 */
}

/* Application Level Lora RX */
void Application_LoraRxData( lora_AppData_t *AppData )
{
 // set_at_receive(AppData->Port, AppData->Buff, AppData->BuffSize);
	GMX_Led(LED_2, LED_FLASH_3 );
}


// Here we have 3 additional callbacks we have added to the original code - to let the application know what it's going on

// As name says - when network is Joined this callback is fired - used for LED UI
void Application_LoraJoinedNetwork() {
	network_joined = true;
	_app.state |= APPLICATION_NETWORK_JOINED;

	// LED UI to show network has been joined
	GMX_Led(LED_3, LED_ON );
}


// Callback when we start a Join Request - from Lora FSM - the LoRaMac will continue to retransmit until network is joined
// Take into account that the CalculateBackOff ( LoRaMac.c line 2745 ), will generate a duty cycle based on the first JoinRequest - from RetransmissionDutyCylce() function line 2721
// We have patched the function adding a LoraMacFirstJoinTimeStamp variable with which the delta-t is calculated... after 1 hour the dutycyle goes to 0.1% and after 1 day to 0.01%

void Application_LoraJoinRequest(uint32_t join_requets_num) {

	// LED UI - flashing led - to show in join mode
	GMX_Led(LED_3, LED_FLASH_MEDIUM );
}

// This is a low-level CallBack just before the Radio TX - it passes the TxDelay (ms) due to duty-cycle
// can be used in application to understand if packet has been really sent or not

void Application_LoraMacSendOnChannel(uint32_t tx_delay) {
	if ( tx_delay == 0 )
	{
		GMX_LedFlashHW(LED_1, FLASH_FAST_MS, 1);
		GMX_Led(LED_2,LED_OFF);
	}
	else
	{
        if ( tx_delay > 600000)
        	GMX_Led(LED_2,LED_FLASH_SLOW);
        else
        	GMX_Led(LED_2,LED_FLASH_FAST);

		_app.TxDelay = tx_delay;
	}
}
