/* _____  _____  __  __             _____  _____ 
  / ____||_   _||  \/  |    /\     / ____||_   _|
 | |  __   | |  | \  / |   /  \   | (___    | |  
 | | |_ |  | |  | |\/| |  / /\ \   \___ \   | |  
 | |__| | _| |_ | |  | | / ____ \  ____) | _| |_ 
  \_____||_____||_|  |_|/_/    \_\|_____/ |_____|
  (c) 2017 GIMASI SA                                               

 * hw_eeprom.h
 *
 *  Created on: Feb 20, 2017
 *      Author: Massimo Santoli
 */

#ifndef HW_EEPROM_H_
#define HW_EEPROM_H_


HAL_StatusTypeDef writeEEPROMWord(uint32_t address, uint32_t data);
uint32_t readEEPROMWord(uint32_t address);
HAL_StatusTypeDef writeEEPROMByte(uint32_t address, uint8_t data);
uint8_t readEEPROMByte(uint32_t address);



#endif /* CORE_INC_HW_EEPROM_H_ */
