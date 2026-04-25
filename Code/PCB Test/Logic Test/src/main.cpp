#include <Arduino.h>

#define MAD_IN_PIN 0
#define WRITE_PIN 1

// #define RST_PIN 2
// #define X_PIN 3
// #define Y_PIN 4

const pin_size_t bus[] = {2,3,4,5,6,7,8,9};

// class Pulse{
//   private:
//     pin_size_t pin;

//   public:
//     volatile uint32_t rise_time;
//     volatile uint16_t pulse_time;
//     volatile bool pulsed = false;

//     Pulse(pin_size_t pin, voidFuncPtr callback){
//       pinMode(pin, INPUT_PULLDOWN);
//       attachInterrupt(digitalPinToInterrupt(pin), callback, CHANGE);
//     }

//     void callback(){
//       if(digitalRead(pin))
//         rise_time = micros();
//       else {
//         pulse_time = micros() - rise_time;
//         pulsed = true;
//       }
//     }

//     void reset(){
//       pulsed = false;
//     }
// };

// void rst_callback();
// void x_callback();
// void y_callback();

// Pulse rst = Pulse(RST_PIN, rst_callback);
// Pulse x = Pulse(X_PIN, x_callback);
// Pulse y = Pulse(Y_PIN, y_callback);

// void rst_callback(){rst.callback();}
// void x_callback(){x.callback();}
// void y_callback(){y.callback();}

void setup() {
  pinMode(MAD_IN_PIN, OUTPUT);
  pinMode(WRITE_PIN, OUTPUT);

  SerialUSB.setTimeout(4294967295);
  SerialUSB.begin(115200);


  for(pin_size_t pin : bus) {
    pinMode(pin, OUTPUT);
  }
}

void write_to_bus(uint8_t val){
  for(int i = 0; i < 8; i++){
    digitalWrite(bus[i], (val >> i) & 0b1);
  }
}

// void reset_all_pulses(){
//   rst.reset();
//   x.reset();
//   y.reset();
// }

void loop() {
  // reset_all_pulses();
  // digitalWrite(MAD_IN_PIN, HIGH);
  // sleep_ms(1000);
  // digitalWrite(MAD_IN_PIN, LOW);

  // SerialUSB.println("MAD In:");
  // SerialUSB.println("Times:");
  // SerialUSB.print(" - RST: ");
  // SerialUSB.println(rst.pulsed ? String(x.pulse_time) : "Didn't pulse");
  // SerialUSB.println(" - X: " + x.pulsed ? String(x.pulse_time) : "Didn't pulse :(");
  // SerialUSB.println(" - Y: " + y.pulsed ? String(y.pulse_time) : "Didn't pulse :(");
  // if(x.pulsed && y.pulsed)
  //   SerialUSB.println(" - XY Offset: " + String(y.rise_time - x.rise_time));
  SerialUSB.print("Enter an address: ");
  String input = SerialUSB.readStringUntil('\n');
  uint8_t addr = input.toInt();
  SerialUSB.println("Writing " + String(addr) + " to memory...");
  write_to_bus(addr);
  digitalWrite(MAD_IN_PIN, HIGH);
  sleep_ms(20);
  digitalWrite(MAD_IN_PIN, LOW);
  SerialUSB.println("Done!");
  SerialUSB.println();

  // digitalWrite(MAD_IN_PIN, HIGH);
  // sleep_ms(100);
  // digitalWrite(MAD_IN_PIN, LOW);
  // sleep_ms(200);
  // digitalWrite(WRITE_PIN, HIGH);
  // sleep_ms(100);
  // digitalWrite(WRITE_PIN, LOW);
  // sleep_ms(200);
}
