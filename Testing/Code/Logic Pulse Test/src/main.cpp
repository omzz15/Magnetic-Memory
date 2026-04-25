#include <Arduino.h>

#define OUTPUT_PIN 12

void setup() {
  // put your setup code here, to run once:
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(OUTPUT_PIN, HIGH);
  digitalWrite(13, HIGH);

  delayMicroseconds(100);

  digitalWrite(OUTPUT_PIN, LOW);
  digitalWrite(13, LOW);

  delayMicroseconds(1);
}