#include <Arduino.h>
#include <SoftwareInterrupt.h>

#define BAUD_RATE 115200

// Define the pins
#define READ_REQ_PIN P4_1
#define READ_AVAIL_PIN P3_7
#define WRITE_REQ_PIN P8_2

static const uint8_t madXPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
static const uint8_t madYPins[] = {19, 18, 17, 15, 14, 13, 12, 11};
static const uint8_t readBusPins[] = {23, 24, 25, 26, 27, 28, 29, 30};
static const uint8_t writeBusPins[] = {40, 39, 38, 37, 36, 35, 34, 33};

//Other pins
#define FAST_BTN PUSH1 // The button to enable fast mode
#define FAST_LED RED_LED // The LED to indicate fast mode is active
#define ACTIVITY_LED GREEN_LED // The LED to indicate activity


static uint8_t getAddress();
static void writePins(const uint8_t pins[8], byte data);
static void onReadRequest(uint8_t pin, bool state);
static void onWriteRequest(uint8_t pin, bool state);
static void onFastModeChange(uint8_t pin, bool state);

static SoftwareInterrupt read = SoftwareInterrupt(READ_REQ_PIN, &onReadRequest, RISING);
static SoftwareInterrupt write = SoftwareInterrupt(WRITE_REQ_PIN, &onWriteRequest, RISING);
static SoftwareInterrupt fastMode = SoftwareInterrupt(FAST_BTN, &onFastModeChange, RISING);

static byte memory[64];
static bool fastModeEnabled = false;

const void print(const char *chars){
  if(!fastModeEnabled)
    Serial.print(chars);
}

const void print(const uint8_t c){
  if(!fastModeEnabled)
    Serial.print(c);
}

const void println(const char *chars){
  if(!fastModeEnabled)
    Serial.println(chars);
}

const void println(const uint8_t c){
  if(!fastModeEnabled)
    Serial.println(c);
}

void setup()
{
  Serial.begin(BAUD_RATE);

  pinMode(FAST_BTN, INPUT_PULLUP);

  pinMode(FAST_LED, OUTPUT);
  pinMode(ACTIVITY_LED, OUTPUT);

  pinMode(READ_REQ_PIN, INPUT);
  pinMode(READ_AVAIL_PIN, OUTPUT);
  pinMode(WRITE_REQ_PIN, INPUT);

  for (uint8_t i = 0; i < 8; i++)
  {
    pinMode(madXPins[i], INPUT);
    pinMode(madYPins[i], INPUT);
    pinMode(readBusPins[i], OUTPUT);
    pinMode(writeBusPins[i], INPUT);
  }

  Serial.println("[i] Setup done");
}

void loop()
{
  read.update();
  write.update();
  fastMode.update();
}

static uint8_t getAddress()
{
  uint8_t madX = 255;
  uint8_t madY = 255;
  for (uint8_t i = 0; i < 8; i++)
  {
    if (digitalRead(madXPins[i]) == HIGH)
    {
      if (madX != 255)
      {
        println("[-] Multiple X pins are HIGH");
        return 255;
      }
      madX = i;
    }
    if (digitalRead(madYPins[i]) == HIGH)
    {
      if (madY != 255)
      {
        println("[-] Multiple Y pins are HIGH");
        return 255;
      }
      madY = i;
    }
  }
  if (madX == 255 || madY == 255)
  {
    println("[i] No X or Y pin is HIGH. Exiting.");
    return 255;
  }

  return madX + (madY * 8);
}

static void writePins(const uint8_t pins[8], byte data)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    digitalWrite(pins[i], bitRead(data, i));
  }
}

static bool handleReadRequest()
{
  // Get the address
  uint8_t address = getAddress();
  if (address == 255)
    return false;

  print("[i] R ");
  print(address);
  print(" -> ");
  println(memory[address]);

  // Put data on the bus
  byte data = memory[address];
  delayMicroseconds(10); // Simulate the time it takes to read the data
  writePins(readBusPins, data);
  digitalWrite(READ_AVAIL_PIN, HIGH);

  return true;
}

static void onReadRequest(uint8_t pin, bool state)
{
  digitalWrite(ACTIVITY_LED, HIGH);

  delayMicroseconds(1); // Wait for the data to be ready OK
  bool complete = handleReadRequest();
  delayMicroseconds(1); // Wait for the data to be read OK

  // Reset the pins
  digitalWrite(READ_AVAIL_PIN, LOW);
  writePins(readBusPins, 0);
  digitalWrite(ACTIVITY_LED, LOW);

  if (!complete)
    println("[-] Read Failed");
}

static bool handleWriteRequest()
{
  // Get the address
  uint8_t address = getAddress();
  if (address == 255)
    return false;

  // Read the data
  byte data = 0;
  for (uint8_t i = 0; i < 8; i++)
  {
    data |= !digitalRead(writeBusPins[i]) << i; // Invert the data because the write bus is inverted
  }
  memory[address] = data;

  print("[i] W ");
  print(address);
  print(" <- ");
  println(data);
  
  
  delayMicroseconds(10); // Simulate the time it takes to write the data

  return true;
}

static void onWriteRequest(uint8_t pin, bool state)
{
  digitalWrite(ACTIVITY_LED, HIGH);

  bool complete = handleWriteRequest();

  // Reset the pins
  writePins(writeBusPins, 0);
  digitalWrite(ACTIVITY_LED, LOW);

  if (!complete)
    println("[-] Write Failed");
}

static void onFastModeChange(uint8_t pin, bool state)
{
  if(fastModeEnabled)
    Serial.begin(BAUD_RATE);

  fastModeEnabled = !fastModeEnabled;
  Serial.print("[i] Fast mode");
  Serial.println(fastModeEnabled ? " enabled" : " disabled");
  digitalWrite(FAST_LED, fastModeEnabled);

  if(fastModeEnabled)
    Serial.end();
}
