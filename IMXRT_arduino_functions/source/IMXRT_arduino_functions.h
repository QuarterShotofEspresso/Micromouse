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


typedef enum {
	HIGH,
	LOW,
	INPUT,
	OUTPUT
	} arduino_types_t;

/* API */

/*
 * digitalWrite functions as an output signal
 *
 */
void digitalWrite(uint32_t pin, arduino_types_t logicLevel);

/*
 * digitalRead acts as an input
 */
bool digitalRead(uint32_t pin);

/*
 * Analog Write is responsible for writing data
 * in PWM format.
 */
void analogWrite(uint32_t pin, int value);

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
