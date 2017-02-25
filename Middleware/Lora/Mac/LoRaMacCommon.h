/*!
 * \file      LoRaMacCommon.h
 *
 * \brief     LoRa MAC layer implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013 Semtech
 *
 *               ___ _____ _   ___ _  _____ ___  ___  ___ ___
 *              / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 *              \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 *              |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 *              embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Daniel Jäckle ( STACKFORCE )
 *
 * \defgroup  LORAMAC LoRa MAC layer implementation
 *            This module specifies the API implementation of the LoRaMAC layer.
 *            This is a placeholder for a detailed description of the LoRaMac
 *            layer and the supported features.
 * \{
 *
 * \example   classA/LoRaMote/main.c
 *            LoRaWAN class A application example for the LoRaMote.
 *
 * \example   classB/LoRaMote/main.c
 *            LoRaWAN class B application example for the LoRaMote.
 *
 * \example   classC/LoRaMote/main.c
 *            LoRaWAN class C application example for the LoRaMote.
 */
#ifndef __LORAMAC_COMMMON_H__
#define __LORAMAC_COMMMON_H__

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/*!
 * LoRaWAN devices classes definition
 */
typedef enum eDeviceClass
{
    /*!
     * LoRaWAN device class A
     *
     * LoRaWAN Specification V1.0, chapter 3ff
     */
    CLASS_A,
    /*!
     * LoRaWAN device class B
     *
     * LoRaWAN Specification V1.0, chapter 8ff
     */
    CLASS_B,
    /*!
     * LoRaWAN device class C
     *
     * LoRaWAN Specification V1.0, chapter 17ff
     */
    CLASS_C,
}DeviceClass_t;

#ifdef __cplusplus
}
#endif

#endif // __LORAMAC_COMMMON_H__
