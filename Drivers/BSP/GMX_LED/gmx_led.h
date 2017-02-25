/* _____  _____  __  __             _____  _____ 
  / ____||_   _||  \/  |    /\     / ____||_   _|
 | |  __   | |  | \  / |   /  \   | (___    | |  
 | | |_ |  | |  | |\/| |  / /\ \   \___ \   | |  
 | |__| | _| |_ | |  | | / ____ \  ____) | _| |_ 
  \_____||_____||_|  |_|/_/    \_\|_____/ |_____|
  (c) 2017 GIMASI SA                                               

 * gmx_led.h
 *
 *  Created on: Feb 18, 2017
 *      Author: Massimo Santoli
 *      Version: 1.0
 */

#ifndef GMX_LED_H_
#define GMX_LED_H_


#define	LED_OFF				0x00
#define	LED_ON				0x01

#define	LED_FLASH_1			0x11
#define	LED_FLASH_2			0x12
#define	LED_FLASH_3			0x13

#define	LED_FLASH_SLOW		0x21
#define	LED_FLASH_MEDIUM	0x22
#define	LED_FLASH_FAST		0x23

#define FLASH_SLOW_MS		1000
#define FLASH_MEDIUM_MS		 500
#define FLASH_FAST_MS		 250


// HW SPECIFIC DEFINES

#define NUM_LEDS	3

#define __LED_ON 	1
#define __LED_OFF 	0

#define LED_1		0
#define LED_1_PORT	GPIOB
#define LED_1_PIN	GPIO_PIN_7

#define LED_2		1
#define LED_2_PORT	GPIOB
#define LED_2_PIN	GPIO_PIN_6

#define LED_3		2
#define LED_3_PORT 	GPIOB
#define LED_3_PIN	GPIO_PIN_5


void GMX_LedFlashHW(uint8_t num_led, uint8_t period, uint8_t number);
bool GMX_Led(uint8_t num_led, uint8_t state );
void GMX_LedInit();

#endif /* GMX_LED_H_ */
