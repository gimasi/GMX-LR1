  @verbatim
  ******************************************************************************
  *  
  *           Portions COPYRIGHT 2016 STMicroelectronics                       
  *
  * @file    st_readme.txt 
  * @author  MCD Application Team
  * @brief   This file lists the main modification done by STMicroelectronics on
  *          LoRa for integration with STM32Cube solution.
  *          For more details on LoRa implementation on STM32Cube, please refer
  *          to UM2073 "STM32 LORA01 software expansion for STM32Cube "  
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
  @endverbatim

### V1.0.3/01-January-2017 ###
===============================
   +  Read date between 2 successive read time to make sure date is ok

### V1.0.2/15-November-2016 ###
===============================
   +  Corrected 1 bug in LoRaMac-board.h: RX_WND_2_CHANNEL for EU is now back at DR_0
   +  Corrected 1 bug in LoRaMac.c for  dataRate adaptation

### V1.0.1/15-September-2016 ###
===============================
   +  Implements LoRa Mac 4.3.0 from Semteck/StackForce 

### V1.0.0/01-July-2016 ###
===============================
   + First R1.0.0 customized version for STM32Cube solution.
   + Comissioning_template.h in /Conf contains all Lora Ids to connect on LoRa network
         It is provided as a template. It must be moved to /Projects/inc/ as Comissioning.h
   + All files in Conf/src are provided as template and must be copied in /Projects/src. 
   + All files in Conf/inc are provided as template and must be copied in /Projects/inc.
		#if 0 and #endif must be removed to enable the template in the user directory
   + Implements LoRa Mac 4.2.0 from Semteck/StackForce
      + \Lora\Mac\LoRaMac.c : replace floating exponent e3 and e6 by int number
      + \Lora\Utilities\delay.c : cast uint32_t
   + Modified intensively timeServer.c 
   + new low layer interfacing Cube HAL (hw_rtc.c, hw_gpio.c and hw_spi.c)
   + added lora.c as an interface layer to ease product integration


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 