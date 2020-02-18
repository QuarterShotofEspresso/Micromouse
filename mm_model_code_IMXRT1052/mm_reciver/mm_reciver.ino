// ADJUST:
// CHANGE MOTOR DIRECTIONS
// ADD MOTOR DIRECTIONALITY (DONE)
// 

#include <RF24.h>
#include <SPI.h>
//#include <nRF24L01.h>

#define CE_PIN 6
#define CSN_PIN 10
#define MOSI_PIN 11
#define MISO_PIN 12
#define SCK_PIN 13
#define INTERRUPT_PIN 9

// PIN SEQ: 5, 6, 7, 8. CONTROLLER SEQ: 4, 3, 2, 1
#define LEFT_MOTOR_NEG 5
#define LEFT_MOTOR_POS 6
#define RIGHT_MOTOR_POS 8
#define RIGHT_MOTOR_NEG 7

RF24 radio(CE_PIN, CSN_PIN); // CE, CSN
const byte address[6] = "00001";

// radio input
char text[2] = "";
// motorPWMs
int rightPWM = 0;
int leftPWM = 0;

void setup() {
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  // set all pin's pinmodes
  pinMode(LEFT_MOTOR_NEG, OUTPUT);
  pinMode(LEFT_MOTOR_POS, OUTPUT);
  pinMode(RIGHT_MOTOR_NEG, OUTPUT);
  pinMode(RIGHT_MOTOR_POS, OUTPUT);

  // set writes and reads to low
  digitalWrite(LEFT_MOTOR_NEG, LOW);
  digitalWrite(RIGHT_MOTOR_NEG, LOW);
  
  // attach the interrupt
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), radioISR, FALLING);
  
}

void loop() {

  leftPWM = (int)text[0];
  rightPWM = (int)text[1];
  
  if (leftPWM > 127) {
    digitalWrite(LEFT_MOTOR_POS, LOW);
    analogWrite(LEFT_MOTOR_NEG, map(leftPWM, 128, 255, 0, 255));
  } else {
    digitalWrite(LEFT_MOTOR_NEG, LOW);
    analogWrite(LEFT_MOTOR_POS, map(leftPWM, 0, 127, 0, 255));
  }

  if (rightPWM > 127) {
    digitalWrite(RIGHT_MOTOR_POS, LOW);
    analogWrite(RIGHT_MOTOR_NEG, map(rightPWM, 128, 255, 0, 255));
  } else {
    digitalWrite(RIGHT_MOTOR_NEG, LOW);
    analogWrite(RIGHT_MOTOR_POS, map(rightPWM, 0, 127, 0, 255));
  }
  
}

void radioISR() {
    radio.read(&text, sizeof(text));
}
