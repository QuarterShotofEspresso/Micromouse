/*
 * Arduino Functionality
 * Functions of the IMXRT are rather complex, and most functionality isn't necessary for the intent
 * and purposes of micromouse.
 */
 
 
#ifndef _ARDUINODEFAULTFUNCTION_H_
#define _ARDUINODEFAULTFUNCTION_H_

typdef enum {HIGH, LOW}LOGIC_LEVEL;


/*
 
 */
void digitalWrite(uint32_t pin, LOGIC_LEVEL logicLevel);

/*
 
 */
bool digitalRead(uint32_t pin);

/*
 Analog Write is responsible for writing data
 in PWM format.
 */
void analogWrite(int pin, int value);

/*
 
 */
int analogRead(int pin);
 
#endif /* _ARDUINODEFAULTFUNCTION_H_ */
