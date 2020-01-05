/*
 * IMXRT_arduino_functions.h
 *
 *  Created on: Dec 26, 2019
 *      Author: espresso
 */

#ifndef IMXRT_ARDUINO_FUNCTIONS_H_
#define IMXRT_ARDUINO_FUNCTIONS_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MIMXRT1052.h"
#include "fsl_debug_console.h"


enum arduino_types {
	HIGH,
	LOW,
	INPUT,
	OUTPUT
	};

/* API */

// @param: pin - pin ids are found in the IMXRT programmer's manual

/*
 * Function: digitalWrite
 * @brief: digitalWrite function sets the pad identified by pin
 * to an output signal specified by logicLevel
 *
 */
void digitalWrite(uint32_t pin, arduino_types_t logicLevel);

/*
 * Function: digitalRead
 * @brief: 	digitalRead serves as General Purpose Input
 * @return: returns a 1 or 0 depending on the voltage level supplied
 * 			at the input pin
 */
bool digitalRead(uint32_t pin);

/*
 * @brief: 	AnalogWrite produces a PWM signal at pad specified by pin
 * 			and the duty cycle is computed using dutyCycle
 * @param:	dutyCycle
 */
void analogWrite(uint32_t pin, int dutyCycle);

/*
 * analogRead uses the ADC from the imxrt and produces an integer value
 * between 0 and 1023
 */
int analogRead(uint32_t pin);

/*
 * pinMode sets the direction of data traveling through the pin
 *
 */
void pinMode(uint32_t pin, arduino_types_t dataDirection);

#endif /* IMXRT_ARDUINO_FUNCTIONS_H_ */
