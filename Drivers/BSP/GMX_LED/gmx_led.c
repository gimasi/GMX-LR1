/*
   _____  _____  __  __             _____  _____
  / ____||_   _||  \/  |    /\     / ____||_   _|
 | |  __   | |  | \  / |   /  \   | (___    | |
 | | |_ |  | |  | |\/| |  / /\ \   \___ \   | |
 | |__| | _| |_ | |  | | / ____ \  ____) | _| |_
  \_____||_____||_|  |_|/_/    \_\|_____/ |_____|
      (c) 2017 Gimasi SA
 *
 *  Created on: 	Feb 18, 2017
 *      Author: 	Massimo Santoli
 *      Version: 	1.0
 *      Brief:		Led Driver for GMX-LR1 Board
 *      			Easy way to flash and handle LED Events
 *      			Lot of space for optimization
 *
 */

#include "hw.h"
#include "stdbool.h"
#include "delay.h"
#include "timeServer.h"
#include "gmx_led.h"

// mask to enable disable led
static uint8_t led_mask;

// led state
//	ON
//	OFF
//  FLASHING NUM OF FLASHES
//  FLASHING ALLWAYS

#define FLASH_COUNT_INFINITE		10

static uint8_t led_status[NUM_LEDS];
static uint8_t led_hw_status[NUM_LEDS];
static uint8_t led_force_status[NUM_LEDS];
static uint8_t led_flash_counts[NUM_LEDS];


static uint16_t led_pins[NUM_LEDS];
static GPIO_TypeDef *led_ports[NUM_LEDS];
static TimerEvent_t led_timers[NUM_LEDS];
static uint32_t led_flash_speed[NUM_LEDS];

static void OnLed1TimerEvent( void );
static void OnLed2TimerEvent( void );
static void OnLed3TimerEvent( void );

static void __GMX_updateHW(uint8_t i);

void GMX_LedInit() {

	// init underlyning HW
	GPIO_InitTypeDef initStruct={0};

	initStruct.Mode =GPIO_MODE_OUTPUT_PP;
	initStruct.Pull = GPIO_NOPULL;
	initStruct.Speed = GPIO_SPEED_HIGH;


#ifdef LED_1_PORT
	led_pins[LED_1]=LED_1_PIN;
	led_ports[LED_1]=LED_1_PORT;
	HW_GPIO_Init( LED_1_PORT, LED_1_PIN, &initStruct  );
#endif

#ifdef LED_2_PORT
	led_pins[LED_2]=LED_2_PIN;
	led_ports[LED_2]=LED_2_PORT;
	HW_GPIO_Init( LED_2_PORT, LED_2_PIN, &initStruct  );
#endif

#ifdef LED_3_PORT
	led_pins[LED_3]=LED_3_PIN;
	led_ports[LED_3]=LED_3_PORT;
	HW_GPIO_Init( LED_3_PORT, LED_3_PIN, &initStruct  );
#endif


	// init HAL

	led_mask = 0xff;

	for(int i=0;i<NUM_LEDS;i++) {
		led_status[i] = led_force_status[i] = led_flash_counts[i] = LED_OFF;
		HW_GPIO_Write( led_ports[i], led_pins[i],__LED_OFF );
	}

	TimerInit( &led_timers[0], OnLed1TimerEvent );
	led_flash_speed[0] = FLASH_SLOW_MS;


	TimerInit( &led_timers[1], OnLed2TimerEvent );
	led_flash_speed[1] = FLASH_SLOW_MS;

	TimerSetValue( &led_timers[1], FLASH_SLOW_MS );

	TimerInit( &led_timers[2], OnLed3TimerEvent );
	led_flash_speed[2] = FLASH_SLOW_MS;

	TimerSetValue( &led_timers[2], FLASH_SLOW_MS );

}


bool GMX_Led(uint8_t num_led, uint8_t state ) {


	if (num_led > NUM_LEDS-1)
		return false;

	switch(state)
	{
		case LED_ON:
			led_status[num_led] = state;
			led_flash_counts[num_led] = 0;
			break;
		case LED_OFF:
			led_status[num_led] = state;
			led_flash_counts[num_led] = 0;
			break;
		case LED_FLASH_1:
			led_status[num_led] = state;
			led_flash_counts[num_led] = 1;
			break;
		case LED_FLASH_2:
			led_status[num_led] = state;
			led_flash_counts[num_led] = 3;
			break;
		case LED_FLASH_3:
			led_status[num_led] = state;
			led_flash_counts[num_led] = 5;
			break;
		case LED_FLASH_SLOW:
		case LED_FLASH_MEDIUM:
		case LED_FLASH_FAST:
			led_status[num_led] = state;
			led_flash_counts[num_led] = FLASH_COUNT_INFINITE;
			break;
		default:
			return false;
	}

	__GMX_updateHW(num_led);

	return true;

}

// blocking function to flash led
void GMX_LedFlashHW(uint8_t num_led, uint8_t period, uint8_t number) {
	uint8_t i;

	if (num_led > NUM_LEDS-1)
			return;

	for (i=0;i<number;i++) {
		HW_GPIO_Write( led_ports[num_led], led_pins[num_led], __LED_ON);
		DelayMs(period);
		HW_GPIO_Write( led_ports[num_led], led_pins[num_led], __LED_OFF);
		DelayMs(period);
	}
}





void GMX_ledMask(uint8_t mask) {
	led_mask = mask;
	__GMX_updateHW(0);
}


static void OnLed1TimerEvent( void ){
	uint32_t state;
	uint8_t hw_state;

	// LED timer change state
	state = HW_GPIO_Read( led_ports[LED_1], led_pins[LED_1] );

	hw_state = state == __LED_OFF ?__LED_ON : __LED_OFF;

	HW_GPIO_Write( led_ports[LED_1], led_pins[LED_1], hw_state);

	if ( led_flash_counts[LED_1] != FLASH_COUNT_INFINITE )
		led_flash_counts[LED_1]--;

	if ( led_flash_counts[LED_1] > 0 ) {

		TimerSetValue( &led_timers[LED_1], led_flash_speed[LED_1] );
		TimerStart(&led_timers[LED_1]);
	}
	else {
		led_status[LED_1] = LED_OFF;
		TimerStop(&led_timers[LED_1]);
	}

}

static void OnLed2TimerEvent( void ){
	uint32_t state;
		uint8_t hw_state;

		// LED timer change state
		state = HW_GPIO_Read( led_ports[LED_2], led_pins[LED_2] );

		hw_state = state == __LED_OFF ?__LED_ON : __LED_OFF;

		HW_GPIO_Write( led_ports[LED_2], led_pins[LED_2], hw_state);

		if ( led_flash_counts[LED_2] != FLASH_COUNT_INFINITE )
			led_flash_counts[LED_2]--;

		if ( led_flash_counts[LED_2] > 0 ) {
			TimerSetValue( &led_timers[LED_2], led_flash_speed[LED_2] );
			TimerStart(&led_timers[LED_2]);
		}
		else {
			led_status[LED_2] = LED_OFF;
			TimerStop(&led_timers[LED_2]);
		}

}

static void  OnLed3TimerEvent(void) {
	uint32_t state;
		uint8_t hw_state;

		// LED timer change state
		state = HW_GPIO_Read( led_ports[LED_3], led_pins[LED_3] );

		hw_state = state == __LED_OFF ?__LED_ON : __LED_OFF;

		HW_GPIO_Write( led_ports[LED_3], led_pins[LED_3], hw_state);

		if ( led_flash_counts[LED_3] != FLASH_COUNT_INFINITE )
			led_flash_counts[LED_3]--;

		if ( led_flash_counts[LED_3] > 0 ) {
			TimerSetValue( &led_timers[LED_3], led_flash_speed[LED_3] );
			TimerStart(&led_timers[LED_3]);
		}
		else {
			led_status[LED_3] = LED_OFF;
			TimerStop(&led_timers[LED_3]);
		}
}

static void __GMX_updateHW(uint8_t i) {
	uint8_t state;

	if ( 1<<i && led_mask) {

			state = led_force_status[i] != LED_OFF ? led_force_status[i] : led_status[i];

			switch(state)
			{
					case LED_ON:
						HW_GPIO_Write( led_ports[i], led_pins[i], __LED_ON);
						led_hw_status[i] = LED_ON;
						TimerStop(&led_timers[i]);
						break;

					case LED_OFF:
						HW_GPIO_Write( led_ports[i], led_pins[i], __LED_OFF);
						led_hw_status[i] = LED_OFF;
						TimerStop(&led_timers[i]);
						break;

					case LED_FLASH_1:
						HW_GPIO_Write( led_ports[i], led_pins[i], __LED_ON);
						led_hw_status[i] = LED_ON;
						led_flash_speed[i]=FLASH_SLOW_MS;
						TimerSetValue(&led_timers[i],FLASH_SLOW_MS);
						TimerStart(&led_timers[i]);
						break;

					case LED_FLASH_2:
						HW_GPIO_Write( led_ports[i], led_pins[i], __LED_ON);
						led_hw_status[i] = LED_ON;
						led_flash_speed[i]=FLASH_SLOW_MS;
						TimerSetValue(&led_timers[i],FLASH_SLOW_MS);
						TimerStart(&led_timers[i]);
						break;

					case LED_FLASH_3:
						HW_GPIO_Write( led_ports[i], led_pins[i], __LED_ON);
						led_hw_status[i] = LED_ON;
						led_flash_speed[i]=FLASH_SLOW_MS;
						TimerSetValue(&led_timers[i],FLASH_SLOW_MS);
						TimerStart(&led_timers[i]);
						break;

					case LED_FLASH_SLOW:
						HW_GPIO_Write( led_ports[i], led_pins[i], __LED_ON);
						led_hw_status[i] = LED_ON;
						led_flash_speed[i]=FLASH_SLOW_MS;
						TimerSetValue(&led_timers[i],FLASH_SLOW_MS);
						TimerStart(&led_timers[i]);
						break;

					case LED_FLASH_MEDIUM:
						HW_GPIO_Write( led_ports[i], led_pins[i], __LED_ON);
						led_hw_status[i] = LED_ON;
						led_flash_speed[i]=FLASH_MEDIUM_MS;
						TimerSetValue(&led_timers[i],FLASH_MEDIUM_MS);
						TimerStart(&led_timers[i]);
						break;

					case LED_FLASH_FAST:
						HW_GPIO_Write( led_ports[i], led_pins[i], __LED_ON);
						led_hw_status[i] = LED_ON;
						led_flash_speed[i]=FLASH_FAST_MS;
						TimerSetValue(&led_timers[i],FLASH_FAST_MS);
						TimerStart(&led_timers[i]);
						break;
			}


		} else {
			HW_GPIO_Write( led_ports[i], led_pins[i], __LED_OFF);
			led_hw_status[i] = LED_OFF;
		}


}
