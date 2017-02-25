/* _____  _____  __  __             _____  _____ 
  / ____||_   _||  \/  |    /\     / ____||_   _|
 | |  __   | |  | \  / |   /  \   | (___    | |  
 | | |_ |  | |  | |\/| |  / /\ \   \___ \   | |  
 | |__| | _| |_ | |  | | / ____ \  ____) | _| |_ 
  \_____||_____||_|  |_|/_/    \_\|_____/ |_____|
  (c) 2017 GIMASI SA                                               

 * application.h
 *
 *  Created on: Feb 19, 2017
 *      Author: Massimo Santoli
 *      Version: 1.0
 */

#ifndef APPLICATION_USER_APPLICATION_H_
#define APPLICATION_USER_APPLICATION_H_


#define APPLICATION_NAME "LORAWAN APPLICATION"

#define APPLICATION_VERSION_MAJOR 0
#define APPLICATION_VERSION_MINOR 9
#define APPLICATION_VERSION (uint16_t) (APPLICATION_VERSION_MAJOR << 8) | (APPLICATION_VERSION_MINOR )


typedef struct sLoraConfig {

	uint8_t appSKey[16];
	uint8_t nwkSKey[16];
	uint8_t devEui[8];
	uint8_t appEui[8];
	uint8_t appKey[16];
	uint32_t devAddr;

	uint8_t ota;
	uint8_t port;
	uint8_t mode;
	uint8_t drDef;
	uint8_t drMax;
	uint8_t drMin;
	uint8_t power;

} LoraConfig_t;


// application states

typedef enum eApplicationState
{
    APPLICATION_IDLE = 0x0000,

	/* Here you define user States */
	APPLICATION_NETWORK_JOINED = 0x0001,
	APPLICATION_REJOIN_NETWORK = 0x0002,
	APPLICATION_TX_PACKET = 0x0004

} ApplicationState_t;


typedef struct sApplication
{

	ApplicationState_t state;
	uint32_t		   TxDelay;

	/* Here other application specific variables */

} Application_t;


void Application_Init();
void Application_Loop();
ApplicationState_t Application_getApplicationState();

void Application_LoraTxData(lora_AppData_t *AppData, FunctionalState *IsTxConfirmed);
void Application_LoraRxData(lora_AppData_t *AppData);
void Application_LoraJoinedNetwork();
void Application_LoraJoinRequest(uint32_t join_requets_num);
void Application_LoraMacSendOnChannel(uint32_t tx_delay);


#endif /* APPLICATION_USER_APPLICATION_H_ */
