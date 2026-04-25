#include <Arduino.h>

#define RST_PIN 4
#define READ_OUT_PIN 5
#define BUS_PIN 6

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
  pinMode(READ_OUT_PIN, OUTPUT);
  pinMode(BUS_PIN, INPUT);
}

void loop() {
  digitalWrite(RST_PIN, HIGH);
  delayMicroseconds(1000);
  digitalWrite(RST_PIN, LOW);
  delayMicroseconds(400);
  digitalWrite(READ_OUT_PIN, HIGH);
  delayMicroseconds(100);
  bool signal = digitalRead(BUS_PIN);
  digitalWrite(READ_OUT_PIN, LOW);

  if(signal){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5);
    digitalWrite(LED_BUILTIN, LOW);
  }
}