#include <Arduino.h>

#define PULSE_PIN 2
#define PULSE_PIN_2 3

#define PULSE_WIDTH 10
#define DELAY 200

// uint8_t risePerCycle = 1;
// uint8_t pulseDelay = 1;

// class SmoothChangePin{
//   private:
//     uint8_t pin;
//     uint8_t risePerCycle;
//     uint8_t target;
//     uint8_t current;
//   public:
//     SmoothChangePin(uint8_t pin, uint8_t risePerCycle){
//       this->pin = pin;
//       this->risePerCycle = risePerCycle;
//       pinMode(pin, OUTPUT);
//     }
//     void update(){
//       if(current < target){
//         current += risePerCycle;
//         if(current > target)
//           current = target;
//       } else if(current > target){
//         current -= risePerCycle;class SmoothChangePin{
//   private:
//     uint8_t pin;
//     uint8_t risePerCycle;
//     uint8_t target;
//     uint8_t current;
//   public:
//     SmoothChangePin(uint8_t pin, uint8_t risePerCycle){
//       this->pin = pin;
//       this->risePerCycle = risePerCycle;
//       pinMode(pin, OUTPUT);
//     }
//     void update(){
//       if(current < target){
//         current += risePerCycle;
//         if(current > target)
//           current = target;
//       } else if(current > target){
//         current -= risePerCycle;
//         if(current < target)
//           current = target;
//       }
//       analogWrite(pin, current);
//     }
//     void setRisePerCycle(uint8_t risePerCycle){
//       this->risePerCycle = risePerCycle;
//     }
//     void setTarget(uint8_t target){
//       this->target = target;
//     }
//     uint8_t getTarget(){
//       return target;
//     }
//     void forceSetCurrent(uint8_t current){
//       this->current = current;
//       this->target = current;
//       analogWrite(pin, current);
//     }
//     uint8_t getCurrent(){
//       return current;
//     }
//     bool isDone(){
//       return current == target;
//     }
// };
//         if(current < target)
//           current = target;
//       }
//       analogWrite(pin, current);
//     }
//     void setRisePerCycle(uint8_t risePerCycle){
//       this->risePerCycle = risePerCycle;
//     }
//     void setTarget(uint8_t target){
//       this->target = target;
//     }
//     uint8_t getTarget(){
//       return target;
//     }
//     void forceSetCurrent(uint8_t current){
//       this->current = current;
//       this->target = current;
//       analogWrite(pin, current);
//     }
//     uint8_t getCurrent(){
//       return current;
//     }
//     bool isDone(){
//       return current == target;
//     }
// };

// SmoothChangePin pin(PULSE_PIN, risePerCycle);
// SmoothChangePin pin2(PULSE_2_PIN, risePerCycle);

void pulse(uint8_t pin, int length);
// void smoothPulse(SmoothChangePin pin);

void setup() {
  // Serial.begin(115200);

  pinMode(PULSE_PIN, OUTPUT);
  pinMode(PULSE_PIN_2, OUTPUT);

  // pinMode(PUSH1, INPUT_PULLUP);
  // pinMode(RED_LED, OUTPUT);

  // Serial.println("Ready");
}

void loop() {
  // if(Serial.available()) {
  //   int length = Serial.parseInt();
  //   Serial.readStringUntil('\n'); // read the newline character
  //   genPulse(length);
  //   Serial.println("Pulse sent");
  // }

  // if(digitalRead(PUSH1) == LOW && !wasPressed) {
  //   digitalWrite(RED_LED, HIGH);
  //   wasPressed = true;
  //   genPulse(pulseWidth);
  // } else if(digitalRead(PUSH1) == HIGH && wasPressed) {
  //   digitalWrite(RED_LED, LOW);
  //   wasPressed = false;
  // }

  // genPulse(PULSE_PIN, pulseWidth);
  // delayMicroseconds(1000);
  // genPulse(PULSE_PIN, pulseWidth);
  // delayMicroseconds(1000);
  // genPulse(PULSE_2_PIN, pulseWidth);
  // delayMicroseconds(1000);
  // genPulse(PULSE_2_PIN, pulseWidth);
  // delayMicroseconds(1000);

  pulse(PULSE_PIN, PULSE_WIDTH);
  delayMicroseconds(DELAY);
  pulse(PULSE_PIN, PULSE_WIDTH);
  delayMicroseconds(DELAY);
  pulse(PULSE_PIN_2, PULSE_WIDTH);
  delayMicroseconds(DELAY);
  pulse(PULSE_PIN_2, PULSE_WIDTH);
  delayMicroseconds(DELAY);
}

void pulse(uint8_t pin, int length) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(length);
  digitalWrite(pin, LOW);
}

// void smoothPulse(SmoothChangePin pin) {
//   pin.setTarget(255);
//   while(!pin.isDone()){
//     pin.update();
//     delayMicroseconds(16);
//   }
  
//   pin.setTarget(0);
//   while(!pin.isDone()){
//     pin.update();
//     delayMicroseconds(16);
//   }
// }