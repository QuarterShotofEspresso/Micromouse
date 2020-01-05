// Implementation of IMXRT_arduino_functions
// includes
#include "IMXRT_arduino_functions.h"

/*
 * IMXRT_arduino_functions.c
 *
 *  Created on: Dec 26, 2019
 *      Author: espresso
 */

/* Additional defines */
#define GPIO_ARDUINO_BASE_ADDRESS GPIO2

/* Pin request memory from called pins */


void digitalWrite(uint32_t pin, enum arduino_types logicLevel) {

	uint32_t GDIR_pin_extraction_mask = 1;

	// GDIR is Output if Direction bit is set to 1
	if (GPIO2->GDIR & (GDIR_pin_extraction_mask << (pin)) == 0) {
		// If the direction is misused the function is exited
		return;
	}

	switch (logicLevel) {

		case HIGH :
			GPIO_PinWrite(GPIO2, pin, 1U);
			break;

		case LOW :
			GPIO_PinWrite(GPIO2, pin, 0U);
			break;

		default :
			break;

	}


}



bool digitalRead(uint32_t pin) {

	uint32_t GDIR_pin_extraction_mask = 1;

	// GDIR is Input if Direction bit is set to 0
	if (GPIO2->GDIR & (GDIR_pin_extraction_mask << (pin)) != 0) {
		// If the direction is misused the function is exited
		return;
	}

	return GPIO_PinRead(GPIO_ARDUINO_BASE_ADDRESS, pin);
}



void analogWrite(uint32_t pin, int value) {

}



int analogRead(uint32_t pin) {

}



void pinMode(uint32_t pin, enum arduino_types dataDirection) {

	if (dataDirection == INPUT) {
		gpio_pin_config_t new_pin_config = {kGPIO_DigitalInput, 0, kGPIO_NoIntmode};
	}

	else if (dataDirection == OUTPUT) {
		gpio_pin_config_t new_pin_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
	}

	else {
		return;
	}

	GPIO_PinInit(GPIO_ARDUINO_BASE_ADDERESS, pin, &new_pin_config);

	return;
}
