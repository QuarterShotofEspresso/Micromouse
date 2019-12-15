/* parameters for updating all constants */
#define KP_ML_DATA_INPUT_PIN A1
#define KI_ML_DATA_INPUT_PIN A2
#define KD_ML_DATA_INPUT_PIN A3

#define KP_MR_DATA_INPUT_PIN A4
#define KI_MR_DATA_INPUT_PIN A5
#define KD_MR_DATA_INPUT_PIN A6

// IR reciver pairs inputs
#define LEFT_IR_PIN A7
#define MIDD_IR_PIN A8
#define RIGH_IR_PIN A9

#define LEFT_IR_EXPEC
#define MIDD_IR_EXPEC
#define RIGH_IR_EXPEC

#define RM_PWM_PIN  A10
#define LM_PWM_PIN  A11

/* Additional constants used across multiple functions */
int rightMotorPWM = 0, leftMotorPWM = 0;
double kp_l, ki_l, kd_l, kp_r, ki_r, kd_r;
int error, t;

 /*
  *  brief: function stores values between 0 to 1 as floating point values 
  *  because all constants are recorded as values from a pot-aided voltage divider.
  */
void updatekPID (void) {
  
  kp_l = analogRead(KP_ML_DATA_INPUT_PIN) / 1023.0;
  ki_l = analogRead(KI_ML_DATA_INPUT_PIN) / 1023.0;
  kd_l = analogRead(KD_ML_DATA_INPUT_PIN) / 1023.0;
  
  kp_r = analogRead(KP_MR_DATA_INPUT_PIN) / 1023.0;
  ki_r = analogRead(KI_MR_DATA_INPUT_PIN) / 1023.0;
  kd_r = analogRead(KD_MR_DATA_INPUT_PIN) / 1023.0;

  return;
}

/*
 * brief: determines the adjustment required to correct the PWM of both motors
 */
void pidWrite (void) {
  
  static int errorPrev = 0;       // stores previous error measurement. Initialized to 0 for first measurement
  static int tPrev = 0;           // stores previous error measurement. Initialized to 0 for first measurement
  static int errorSum = error;    // stores previous error measurement. Initialized to error for first measurement
  
  rightMotorPWM += (kp_l * error) + (ki_l * (errorSum * (t - tPrev))) + (kd_l * (error - errorPrev)/(t - tPrev));
  leftMotorPWM  += (kp_r * error) + (ki_r * (errorSum * (t - tPrev))) * (kd_r * (error - errorPrev)/(t - tPrev));

  analogWrite(RM_PWM_PIN, rightMotorPWM);
  analogWrite(LM_PWM_PIN, leftMotorPWM);
  
  errorPrev = error;
  tPrev = t;
  errorSum += error;

  return;
}

/*
 * brief: takes inputs from IR pains and converts to calculable values
 */
void determineError (void) {
  error = pow(analogRead(LEFT_IR_PIN) - LEFT_IR_EXPEC, 2) + 
          pow(analogRead(MIDD_IR_PIN) - MIDD_IR_EXPEC, 2) + 
          pow(analogRead(RIGH_IR_PIN) - RIGH_IR_EXPEC, 2);

  return;
}

void setup (void) {
  updatekPID();
}


void loop (void) {
  determineError();
  pidWrite();
}
