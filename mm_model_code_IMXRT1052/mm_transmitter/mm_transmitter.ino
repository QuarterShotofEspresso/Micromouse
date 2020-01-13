#include <RF24.h>
#include <SPI.h>

#define CE_PIN 34
#define CSN_PIN 10
#define MOSI_PIN 11
#define MISO_PIN 12
#define SCK_PIN 13

#define LEFT_JOYSTICK_PIN 36
#define RIGHT_JOYSTICK_PIN 19

#define CALLIBRATION_INTERRUPT 1

#define DELAY_PERIOD 250 //ms

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";
int leftJoystickOffset, rightJoystickOffset;

void setup() {

  Serial.begin(9600);
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  
  pinMode(LEFT_JOYSTICK_PIN, INPUT);
  pinMode(RIGHT_JOYSTICK_PIN, INPUT);

  leftJoystickOffset = analogRead(LEFT_JOYSTICK_PIN);
  rightJoystickOffset = analogRead(RIGHT_JOYSTICK_PIN);

  attachInterrupt(digitalPinToInterrupt(CALLIBRATION_INTERRUPT), cal_ISR, RISING);
  
}

void loop() {

  int LJV = 0, RJV = 0;

  int leftJoystickValue = analogRead(LEFT_JOYSTICK_PIN);
  int rightJoystickValue = analogRead(RIGHT_JOYSTICK_PIN);

  // print JoystickValues
//  Serial.print("leftJoystickValue: ");
//  Serial.println(leftJoystickValue);
  
  if (leftJoystickValue >= leftJoystickOffset) {
    LJV = map(leftJoystickValue, leftJoystickOffset, 1023, 0, 127); //  ( LJV = Left Joysiick Value )
//    Serial.print("Value Positive LJV: ");
//    Serial.println(LJV);
  }
  
  else
  {
    LJV = map(leftJoystickValue, leftJoystickOffset, 0, 128, 255);
//    Serial.print("Value Negative LJV: ");
//    Serial.println(LJV);
  }

  // print joystickvalues
//  Serial.print("rightJoystickValue: ");
//  Serial.println(rightJoystickValue);

  if (rightJoystickValue >= rightJoystickOffset) {
    RJV = map(rightJoystickValue, rightJoystickOffset, 1023, 0, 127); // ( RJV = Right Joystick Value )
//    Serial.print("Value Positive RJV: ");
//    Serial.println(RJV);
  } 
  
  else
  {
    RJV = map(rightJoystickValue, rightJoystickOffset, 0, 128, 255);
//    Serial.print("Value Negative RJV: ");
//    Serial.println(RJV);
  }
  
  char charLJV = (char)LJV;
  char charRJV = (char)RJV;

  const char text[] = {charLJV, charRJV};
  radio.write(&text, sizeof(text));
  
  delay(DELAY_PERIOD);
}

void cal_ISR() {

  leftJoystickOffset = analogRead(LEFT_JOYSTICK_PIN);
  rightJoystickOffset = analogRead(RIGHT_JOYSTICK_PIN);
  
}
