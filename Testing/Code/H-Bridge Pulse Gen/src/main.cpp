#include <Arduino.h>

// #define LEFT_NEG 2
// #define LEFT_POS 3

// #define RIGHT_NEG 8
// #define RIGHT_POS 9

#define DIR 9

#define ENABLE_PIN 11

// #define STACK_DELAY 0
// #define ENABLE_DELAY 5
#define ON_TIME 100
#define DELAY 100

// void stackedPulse(uint8_t pin1, uint8_t pin2, uint8_t delay, uint16_t onTime){
//   digitalWrite(pin1, HIGH);
//   // delayMicroseconds(delay);
//   digitalWrite(pin2, HIGH);
//   delayMicroseconds(ENABLE_DELAY);
//   digitalWrite(ENABLE_PIN, HIGH);

//   delayMicroseconds(onTime);

//   digitalWrite(ENABLE_PIN, LOW);
//   delayMicroseconds(ENABLE_DELAY);
//   digitalWrite(pin1, LOW);
//   // delayMicroseconds(delay);
//   digitalWrite(pin2, LOW);
// }

void setup() {
  pinMode(DIR, OUTPUT);
  // pinMode(LEFT_NEG, OUTPUT);
  // pinMode(LEFT_POS, OUTPUT);
  // pinMode(RIGHT_NEG, OUTPUT);
  // pinMode(RIGHT_POS, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
}

void loop() {
  // stackedPulse(LEFT_NEG, RIGHT_NEG, STACK_DELAY, ON_TIME);
  // delayMicroseconds(DELAY);
  // stackedPulse(LEFT_POS, RIGHT_POS, STACK_DELAY, ON_TIME);
  // delayMicroseconds(DELAY);

  digitalWrite(DIR, LOW);
  digitalWrite(ENABLE_PIN, HIGH);
  delayMicroseconds(ON_TIME);
  digitalWrite(ENABLE_PIN, LOW);
  delayMicroseconds(DELAY);
  
  digitalWrite(DIR, HIGH);
  digitalWrite(ENABLE_PIN, HIGH);
  delayMicroseconds(ON_TIME);
  digitalWrite(ENABLE_PIN, LOW);
  delayMicroseconds(DELAY);

}