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
//double kpLeft, kiLeft, kdLeft, kpRight, kiRight, kdRight;
//int error, errorHead, t;

/* API */

 /*
  *  brief: function stores values between 0 to 1 as floating point values
  *  because all constants are recorded as values from a pot-aided voltage divider.
  */
void InitializekPID (void);

/*
 * brief: determines the adjustment required to correct the PWM of both motors
 *        and adds the modification to both the left and right default PWMs.
 */
void computePIDandUpdate (void);


/*
 * FUNCITON: COMPUTE ERROR
 * @brief: Logs the ideal IR sensor values from the start of the maze.
 *         Error is calculated
 */
void computeError (void);

/*
 * FUNCTION: INVERT LEFT MOTOR DIRECTION
 * @brief: Flips the assigned left motor directions when called
 */
void InvertLEFTmotorDirection(void);

/*
 * FUNCTION: REVERT LEFT MOTOR DIRECTION
 * @brief: Unflips the flipped left motor directions
 */
void RevertLEFTmotorDirection(void);


/*
 * FUNCTION: FLASH LED
 * @brief: FLASH LED FOR onTime ms
 */
void flashLED(int onTime);
