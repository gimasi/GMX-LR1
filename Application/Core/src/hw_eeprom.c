/* _____  _____  __  __             _____  _____ 
  / ____||_   _||  \/  |    /\     / ____||_   _|
 | |  __   | |  | \  / |   /  \   | (___    | |  
 | | |_ |  | |  | |\/| |  / /\ \   \___ \   | |  
 | |__| | _| |_ | |  | | / ____ \  ____) | _| |_ 
  \_____||_____||_|  |_|/_/    \_\|_____/ |_____|
  (c) 2017 GIMASI SA                                               

 * hw_eeprom.c
 *
 *  Created on: Feb 20, 2017
 *      Author: Massimo Santoli
 */

#include "hw.h"

/*!
 * \brief Write EEprom data
 */
HAL_StatusTypeDef writeEEPROMWord(uint32_t address, uint32_t data) {

	HAL_StatusTypeDef status = HAL_ERROR;

    address = address + 0x08080000;
    HAL_FLASHEx_DATAEEPROM_Unlock();  //Unprotect the EEPROM to allow writing
    status = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,address,data);
    HAL_FLASHEx_DATAEEPROM_Lock();  // Reprotect the EEPROM

    return status;
}
/*!
 * \brief Read EEprom data
 */
uint32_t readEEPROMWord(uint32_t address) {
    uint32_t tmp = 0;
    address = address + 0x08080000;
    tmp = *(__IO uint32_t*)address;
    return tmp;
}

HAL_StatusTypeDef writeEEPROMByte(uint32_t address, uint8_t data)
{
	HAL_StatusTypeDef status = HAL_ERROR;

    address = address + 0x08080000;
    HAL_FLASHEx_DATAEEPROM_Unlock();  //Unprotect the EEPROM to allow writing
    status = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,address,data);
    HAL_FLASHEx_DATAEEPROM_Lock();  // Reprotect the EEPROM

    return status;
}

uint8_t readEEPROMByte(uint32_t address)
{
    uint8_t tmp = 0;
    address = address + 0x08080000;
    tmp = *(__IO uint8_t*)address;
    return tmp;
}
