/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    proj_tz_pwm_attempt_0.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MIMXRT1052.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_pwm.h"
/* TODO: insert other definitions and declarations here. */
/* The PWM base address */
#define BOARD_PWM_BASEADDR_SCLK PWM1	// SCLK is the Serial Clock for getting the sampled word ~ 5MHz [TZ]
										// 		NOTE: SCLK is PWM1, PWM_A, sub-module 3; GPIO_A0_B0_10 (D5 on the Arduino-like socket)
										//

#define BOARD_PWM_BASEADDR_MCLK PWM2	// MCLK is the Master Clock needed by the AD7766 SAR 24-bit 192-kHz word-rate ADC ~ 1MHz [TZ]
										//		NOTEL MCLK is PWM4, PWM_A, sub-module 0; GPIO_A0_B1_08 (D3 on the Arduino-like socket)
										//

#define PWM_SRC_CLK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk) 	// N.B. "- {id: IPG_CLK_ROOT.outFreq, value: 150 MHz}"

/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    PRINTF("Greetings lifeforms... \n");
    PRINTF("FlexPWM driver example to get our audio system going... \n");

     /* set up PWM behaviour */
    uint32_t	srcClockFreqInHz = PWM_SRC_CLK_FREQ;


	///////////////////////////////////     SCLK PWM setup + initialization
     	pwm_config_t 	pwmConfig_SCLK; // define config variable for PWM channel (SCLK, MCLK) [TZ]
    	PWM_GetDefaultConfig(&pwmConfig_SCLK);
	    /* enable debug mode [TZ] */
		//pwmConfig_SCLK.enableDebugMode = true;
		/* Initialize PWM1 --- SCLK */
		if (PWM_Init(BOARD_PWM_BASEADDR_SCLK, kPWM_Module_3, &pwmConfig_SCLK) == kStatus_Fail)
		{
			PRINTF("PWM SCLK initialization failed\n");
			return 1;
		}

    	status_t				status_SCLK_setup = kStatus_Fail; 	// variable holds return-value of PWM_Setup() for SCLK
        pwm_signal_param_t*		pwmSignalParam_SCLK = malloc(sizeof(pwm_signal_param_t)); //= {kPWM_PwmA, (uint8_t) 50, kPWM_HighTrue, (uint16_t) 0};

		/* set some signal parameters for the SCLK PWM */
		pwmSignalParam_SCLK -> pwmChannel = kPWM_PwmA; // SCLK uses PWM_A
		pwmSignalParam_SCLK -> dutyCyclePercent = (uint8_t) 50; // want a 50% duty cycle
		pwmSignalParam_SCLK -> level = kPWM_HighTrue; // active high
		pwmSignalParam_SCLK -> deadtimeValue = (uint16_t) 0; // no dead-time insertion
		uint32_t	SCLKFreqInHz=5000000;

		status_SCLK_setup 	= PWM_SetupPwm(	BOARD_PWM_BASEADDR_SCLK,			// PWM 1
											kPWM_Module_3,						// sub-module 3
											pwmSignalParam_SCLK,				// see assignments made to this variable, above
											(uint8_t) 1,						// only need PWM_A
											kPWM_SignedCenterAligned,			// signed-center aligned (see reference manual)
											SCLKFreqInHz,					// 5-MHz clock, divides evenly into 150 MHz
											srcClockFreqInHz);
		if (status_SCLK_setup == kStatus_Fail)
		{
			PRINTF("PWM SCLK setup failed\n");
			return 1;
		}
		/* Set the load okay bit for all submodules to load registers from their buffer */
		PWM_SetPwmLdok(BOARD_PWM_BASEADDR_SCLK, kPWM_Control_Module_3, true);
		/* Start the PWM generation from Submodule 3 */
		PWM_StartTimer(BOARD_PWM_BASEADDR_SCLK, kPWM_Control_Module_3);


	///////////////////////////////////     MCLK PWM setup + initialization
     	pwm_config_t 	pwmConfig_MCLK; // define config variable for PWM channel (SCLK, MCLK) [TZ]
	    PWM_GetDefaultConfig(&pwmConfig_MCLK);

	    /* enable debug mode [TZ] */
		//pwmConfig_MCLK.enableDebugMode = true;
		/* Initialize PWM4 --- MCLK */
		if (PWM_Init(BOARD_PWM_BASEADDR_MCLK, kPWM_Module_3, &pwmConfig_MCLK) == kStatus_Fail)
		{
			PRINTF("PWM MCLK initialization failed\n");
			return 1;
		}

	    status_t				status_MCLK_setup = kStatus_Fail;	// variable holds return-value of PWM_Setup() for MCLK
		pwm_signal_param_t*		pwmSignalParam_MCLK = malloc(sizeof(pwm_signal_param_t));// = {kPWM_PwmA, (uint8_t) 50, kPWM_HighTrue, (uint16_t) 0};

		/* set some signal parameters for the MCLK PWM */
		pwmSignalParam_MCLK -> pwmChannel = kPWM_PwmA; // MCLK uses PWM_A
		pwmSignalParam_MCLK -> dutyCyclePercent = (uint8_t) 50; // want a 50% duty cycle
		pwmSignalParam_MCLK -> level = kPWM_HighTrue; // active high
		pwmSignalParam_MCLK -> deadtimeValue = (uint16_t) 0; // no dead-time insertion
		uint32_t	MCLKFreqInHz=1000000;
		status_MCLK_setup 	= PWM_SetupPwm(	BOARD_PWM_BASEADDR_MCLK,			// PWM 2
											kPWM_Module_3,						// sub-module 3
											pwmSignalParam_MCLK,				// see assignments made to this variable, above
											(uint8_t) 1,						// only need PWM_A
											kPWM_SignedCenterAligned,			// signed-center aligned (see reference manual)
											MCLKFreqInHz,					// 1-MHz clock, divides evenly into 150 MHz
											srcClockFreqInHz);
		if (status_MCLK_setup == kStatus_Fail)
		{
			PRINTF("PWM MCLK setup failed\n");
			return 1;
		}
		/* Set the load okay bit for all submodules to load registers from their buffer */
		PWM_SetPwmLdok(BOARD_PWM_BASEADDR_MCLK, kPWM_Control_Module_3, true);
		/* Start the PWM generation from Submodule 3 */
		PWM_StartTimer(BOARD_PWM_BASEADDR_MCLK, kPWM_Control_Module_3);



    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
    }
    return 0 ;
}
