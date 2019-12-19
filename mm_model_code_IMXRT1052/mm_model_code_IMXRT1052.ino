/* parameters for updating all constants */
#define KP_ML_DATA_INPUT_PIN A1
#define KI_ML_DATA_INPUT_PIN A2
#define KD_ML_DATA_INPUT_PIN A3

#define KP_MR_DATA_INPUT_PIN A4
#define KI_MR_DATA_INPUT_PIN A5
#define KD_MR_DATA_INPUT_PIN A6

// IR reciver pairs inputs
#define LEFT_IR_PIN   A7
#define MIDDLE_IR_PIN A8
#define RIGHT_IR_PIN  A9

#define RM_GND_PIN  
#define RM_PWM_PIN  A10
#define LM_GND_PIN  
#define LM_PWM_PIN  A11

// TEST:: if test runs are sucessful, try concatenating both default PWMs into one
#define RIGHT_MOTOR_DEFAULT_PWM 150
#define LEFT_MOTOR_DEFAULT_PWM  150

#define LED_CONTROL 13

/* Additional constants used across multiple functions */
double kpLeft, kiLeft, kdLeft, kpRight, kiRight, kdRight;
int error, errorHead, t;

 /*
  *  brief: function stores values between 0 to 1 as floating point values 
  *  because all constants are recorded as values from a pot-aided voltage divider.
  */
void InitializekPID (void) {

  // constants, k, for the left motor PID
  kpLeft =  analogRead(KP_ML_DATA_INPUT_PIN) / 1023.0;
  kiLeft =  analogRead(KI_ML_DATA_INPUT_PIN) / 1023.0;
  kdLeft =  analogRead(KD_ML_DATA_INPUT_PIN) / 1023.0;

  // constants, k, for the right motor PID
  kpRight = analogRead(KP_MR_DATA_INPUT_PIN) / 1023.0;
  kiRight = analogRead(KI_MR_DATA_INPUT_PIN) / 1023.0;
  kdRight = analogRead(KD_MR_DATA_INPUT_PIN) / 1023.0;

  return;
}

/*
 * brief: determines the adjustment required to correct the PWM of both motors
 *        and adds the modification to both the left and right default PWMs.
 */
void computePIDandUpdate (void) {
  
  static int errorPrev = 0;       // stores previous error measurement. Initialized to 0 for first measurement
  static int tPrev = 0;           // stores previous error measurement. Initialized to 0 for first measurement
  static int errorSum = error;    // stores previous error measurement. Initialized to error for first measurement
  static int rightMotorPWMadjustment = 0;
  static int leftMotorPWMadjustment = 0;
  
  rightMotorPWMadjustment = (kpLeft * error) + (kiLeft * (errorSum * (t - tPrev))) + (kdLeft * (error - errorPrev)/(t - tPrev));
  leftMotorPWMadjustment  = (kpRight * error) + (kiRight * (errorSum * (t - tPrev))) + (kdRight * (error - errorPrev)/(t - tPrev));
  
  // NOTE:: A feautre of the Integral term in PID is that it maintains memory of what happened in the past.
  // meaning any manufacturing offset within the motor that is left unaccounted for is actually accounted for by the integreal term
  analogWrite(RM_PWM_PIN, RIGHT_MOTOR_DEFAULT_PWM + rightMotorPWMadjustment);
  analogWrite(LM_PWM_PIN, LEFT_MOTOR_DEFAULT_PWM - leftMotorPWMadjustment);
  
  errorPrev = error;  // store the prior error into errorPrev for following D of PID
  tPrev = t;          // store t into tPrev for following PID computation
  t = millis();       // update t
  errorSum += error;  // add current error errorSum for following I of PID

  return;
}


/*
 * FUNCITON: COMPUTE ERROR
 * @brief: Logs the ideal IR sensor values from the start of the maze.
 *         Error is calculated 
 */
void computeError (void) {

  // record ExpectedDelta;
  static int ExpectedDelta = analogRead(LEFT_IR_PIN) - analogRead(RIGHT_IR_PIN);
  
  // compute the error. NOTE::: consider mapping values to tweak/stablize error adjustment rate
  error = ExpectedDelta - (analogRead(LEFT_IR_PIN) - analogRead(RIGHT_IR_PIN));
  
  return;
}

/*
 * FUNCTION: INVERT LEFT MOTOR DIRECTION
 * @brief: Flips the assigned left motor directions when called
 */
void InvertLEFTmotorDirection(void) {

  digitalWrite(LM_PWM_PIN, LOW);
  analogWrite(LM_GND_PIN, LEFT_MOTOR_DEFAULT_PWM);

  return;
}

/*
 * FUNCTION: REVERT LEFT MOTOR DIRECTION
 * @brief: Unflips the flipped left motor directions
 */
void RevertLEFTmotorDirection(void) {
  
  digitalWrite(LM_GND_PIN, LOW);
  analogWrite(LM_PWM_PIN, LEFT_MOTOR_DEFAULT_PWM);

  return;
}

void flashLED(int onTime) {
  
  digitalWrite(LED_CONTROL, HIGH);
  delay(onTime);
  digitalWrite(LED_CONTROL, LOW);

  return;
}

void setup (void) {
  /* 
   *  INITIALIZE ALL GPIO porst to respective purposes
   *  
   */

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
