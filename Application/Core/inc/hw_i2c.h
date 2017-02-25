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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_I2C_H__
#define __HW_I2C_H__

#ifdef __cplusplus
 extern "C" {
#endif


void HW_I2C_Init( void );
void HW_I2C_DeInit( void );
void HW_I2C_Reset( void);

HAL_StatusTypeDef  HW_I2C_ReadRegister(uint8_t ,uint8_t , uint8_t, uint8_t * );
HAL_StatusTypeDef  HW_I2C_WriteRegister(uint8_t ,uint8_t , uint8_t );
HAL_StatusTypeDef  HW_I2C_WriteRegisterMulti(uint8_t ,uint8_t ,uint8_t , uint8_t *);
HAL_StatusTypeDef  HW_I2C_DeviceReady( uint16_t , uint32_t , uint32_t );

HAL_StatusTypeDef  HW_I2C_ReadData(uint8_t addr,uint8_t NbByte , uint8_t *pBuffer );
HAL_StatusTypeDef HW_I2C_WriteData ( uint8_t devAddr,uint8_t NbByte , uint8_t *pBuffer );

void HW_I2C_ClockStretch( void );


#ifdef __cplusplus
}
#endif

#endif  /* __HW_I2C_H__ */








