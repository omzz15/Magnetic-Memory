#include <Arduino.h>
#include <SmoothChangePin.cpp>

// Pin definitions
#define PULSE_PIN 2
#define PULSE_PIN_2 3

// Pulse definitions (in microseconds)
#define PULSE_WIDTH 10
#define DELAY 200

// Only use sofware smoothing if absolutely necessary (its slow) 
// SmoothChangePin pin(PULSE_PIN);
// SmoothChangePin pin2(PULSE_2_PIN);

void pulse(uint8_t pin, int length);
// void smoothPulse(SmoothChangePin pin);

void setup() {
  // Set minimum delay to PWM period (for arduino uno, 256(8-bit PWM)/16MHz(Hardware PWM clock) = 16us (PWM period))
  // SmoothChangePin::minDelay = 16;

  pinMode(PULSE_PIN, OUTPUT);
  pinMode(PULSE_PIN_2, OUTPUT);
}

void loop() {
  pulse(PULSE_PIN, PULSE_WIDTH);
  delayMicroseconds(DELAY);
  pulse(PULSE_PIN, PULSE_WIDTH);
  delayMicroseconds(DELAY);
  pulse(PULSE_PIN_2, PULSE_WIDTH);
  delayMicroseconds(DELAY);
  pulse(PULSE_PIN_2, PULSE_WIDTH);
  delayMicroseconds(DELAY);

  // smoothPulse(pin);
  // delayMicroseconds(DELAY);
  // smoothPulse(pin);
  // delayMicroseconds(DELAY);
  // smoothPulse(pin2);
  // delayMicroseconds(DELAY);
  // smoothPulse(pin2);
  // delayMicroseconds(DELAY);
}

void pulse(uint8_t pin, int length) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(length);
  digitalWrite(pin, LOW);
}

// void smoothPulse(SmoothChangePin pin) {
//   pin.setTargetAndWait(255);
//   pin.setTargetAndWait(0);
// }