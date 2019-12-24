#include "mm_model_code_IMXRT1052.h"

void setup (void) {
  /* 
   *  INITIALIZE ALL GPIO porst to respective purposes
   *  
   */

   /* parameters for updating all constants */
  pinMode(KP_ML_DATA_INPUT_PIN, INPUT);
  pinMode(KI_ML_DATA_INPUT_PIN, INPUT);
  pinMode(KD_ML_DATA_INPUT_PIN, INPUT);

  pinMode(KP_MR_DATA_INPUT_PIN, INPUT);
  pinMode(KI_MR_DATA_INPUT_PIN, INPUT);
  pinMode(KD_MR_DATA_INPUT_PIN, INPUT);

  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(MIDDLE_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT)l
  
  pinMode(RM_GND_PIN, OUTPUT);
  pinMode(RM_PWM_PIN, OUTPUT);
  pinMode(LM_GND_PIN, OUTPUT);
  pinMode(LM_PWM_PIN, OUTPUT);
  
  pinMode(LED_CONTROL, OUTPUT);


  // initialize all constants, k, for PID computations
  /* 
   * NOTE:: Initialze PWMs should ONLY be called once at the beggining. 
   * This is because the PWM can only be 
   */
  InitializekPID();
  
  // instuct user loop and ExpectedDelta is to be recorded
  flashLED(2000);
  
  return;
}


void loop (void) {
  
  computeError();
  computePIDandUpdate();
  
  // while the HEAD IR pair is a particular distance from the wall. Invert the motor's pins
  if (analogRead(MIDDLE_IR_PIN) > HEAD_THRESHOLD) {
    InvertLEFTmotorDirection();
    digitalWrite(LED_CONTROL, HIGH);
  } else {
    RevertLEFTmotorDirection();
    digitalWrite(LED_CONTROL, LOW);
  }
}
