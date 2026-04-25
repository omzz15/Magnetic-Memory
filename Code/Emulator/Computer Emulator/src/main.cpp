#include <Arduino.h>

#define MAD_IN_PIN 10
#define RAM_IN_PIN 11
#define RAM_OUT_PIN 12

#define MAD_DIS 13
#define WRITE_DIS A0

static const uint8_t busPins[] = {2, 3, 4, 5, 6, 7, 8, 9};

static void setPinModes(const uint8_t pins[8], uint8_t mode);
static byte readPins(const uint8_t pins[8]);
static void writePins(const uint8_t pins[8], byte data);

static void setMad(uint8_t mad);
static byte readData(uint8_t mad);
static void writeData(uint8_t mad, byte data);

static void setFastMode(bool fastMode);

static void handleRegularMode();
static void handleFastMode();
static void test();

bool fastMode = false;

void setup()
{
  Serial.begin(115200);
  pinMode(MAD_IN_PIN, OUTPUT);
  pinMode(RAM_IN_PIN, OUTPUT);
  pinMode(RAM_OUT_PIN, OUTPUT);

  pinMode(MAD_DIS, OUTPUT);
  pinMode(WRITE_DIS, OUTPUT);

  setPinModes(busPins, INPUT); // Safety

  Serial.println("Welcome to the Computer RAM emulator! Use the following commands to test your ram:");
  Serial.println();
  Serial.println("\tr <MAD> (Reads from the specified MAD)");
  Serial.println("\tw <MAD> <DATA> (Writes DATA to the specified MAD)");
  Serial.println("\td <FUNCTION> <DISABLE> (Disables or enables the specified FUNCTION (0 for MAD or 1 for Write))");
  Serial.println();
  Serial.println("Or switch to Fast Mode(f) for script interface or Test Mode(t) to auto test RAM.");
}

void loop()
{
  if (fastMode)
  {
    handleFastMode();
  }
  else
  {
    handleRegularMode();
  }
}

static void setPinModes(const uint8_t pins[8], uint8_t mode)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    pinMode(pins[i], mode);
  }
}

static byte readPins(const uint8_t pins[8])
{
  byte data = 0;
  for (uint8_t i = 0; i < 8; i++)
  {
    data |= digitalRead(pins[i]) << i;
  }
  return data;
}

static void writePins(const uint8_t pins[8], byte data)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    digitalWrite(pins[i], bitRead(data, i));
  }
}

static void setMad(uint8_t mad)
{
  // Shift MAD
  mad = (mad & 0b00000111) | (mad & 0b00111000) << 1;
  setPinModes(busPins, OUTPUT);
  writePins(busPins, mad);
  digitalWrite(MAD_IN_PIN, HIGH);
  delayMicroseconds(10); // Wait for MAD to be read OK
  digitalWrite(MAD_IN_PIN, LOW);
}

static byte readData(uint8_t mad)
{
  setMad(mad);
  if(fastMode)
    delayMicroseconds(20); // wait for MAD to be read OK
  else
    delayMicroseconds(1400); // Simulate 1 cycle delay OK
  
  setPinModes(busPins, INPUT);
  digitalWrite(RAM_OUT_PIN, HIGH);
  
  if(fastMode)
    delayMicroseconds(20); // Wait for RAM to be ready OK
  else
    delayMicroseconds(1350); // Wait for RAM to be ready OK

  byte data = readPins(busPins);

  // Reset
  digitalWrite(RAM_OUT_PIN, LOW);

  return data;
}

static void writeData(uint8_t mad, byte data)
{
  setMad(mad);
  if(fastMode)
    delayMicroseconds(20); // wait for MAD to be read OK
  else
    delayMicroseconds(1400); // Simulate 1 cycle delay OK

  writePins(busPins, data);
  digitalWrite(RAM_IN_PIN, HIGH);
  
  if(fastMode)
    delayMicroseconds(30); // Wait for RAM to be ready OK
  else
    delayMicroseconds(1350); // Wait for RAM to be ready OK

  // Reset
  digitalWrite(RAM_IN_PIN, LOW);
  setPinModes(busPins, INPUT);
}

static void setFastMode(bool enabled){
  if(enabled)
    Serial.println("Switching to fast mode. Make sure your memory can support it!");
  fastMode = enabled;
}

void clearSerial()
{
  while (Serial.available())
    Serial.read();
}

static void handleRegularMode()
{
  while (true)
  {
    clearSerial();
    Serial.print("> ");
    while (!Serial.available());
    char command = Serial.read();
    switch (command)
    {
    case 'r':
    {
      uint8_t mad = Serial.parseInt();
      Serial.print("Reading from ");
      Serial.println(mad);
      Serial.print("Data: ");
      Serial.println(readData(mad));
      break;
    }
    case 'w':
    {
      uint8_t mad = Serial.parseInt();
      uint8_t data = Serial.parseInt();
      Serial.print("Writing ");
      Serial.print(data);
      Serial.print(" to ");
      Serial.println(mad);
      writeData(mad, data);
      break;
    }
    case 'd':
    {
      uint8_t function = Serial.parseInt();
      uint8_t disable = Serial.parseInt();
      Serial.print("Setting ");
      Serial.print(function ? "Write" : "MAD");
      Serial.print(" disable to ");
      Serial.println(disable ? "HIGH" : "LOW");
      digitalWrite(function ? WRITE_DIS : MAD_DIS, disable);
      break;
    }
    case 'f':
    {
      setFastMode(true);
      return;
    }
    case 't':
    {
      test();
    }
    default:
    {
      Serial.println("Unknown command. Valid commands are: Read(r), Write(w), Disable(d), Fast Mode(f), and Test Mode(t). Restart for more info.");
      break;
      ;
    }
    }
  }
}

static void handleFastMode()
{
  while (true)
  {
    while (!Serial.available());
    byte data = Serial.read();
    byte command = data & 0b00000011;
    data = data >> 2; // remove command bits

    switch (command)
    {
    case 0:
    { // read
      Serial.write(readData(data));
      break;
    }
    case 1:
    { // write
      while (!Serial.available());
      byte writeVal = Serial.read();
      writeData(data, writeVal);
      Serial.write(0);
      break;
    }
    case 2:
    { // disable
      digitalWrite(data & 1 ? WRITE_DIS : MAD_DIS, bitRead(data, 1));
      Serial.write(0);
      break;
    }
    case 3:
    { // fast mode check and exit
      switch (data & 1)
      {
      case 0:             // fast mode check
        Serial.write(10); // magic number
        break;
      case 1: // exit
        fastMode = false;
        return;
      }
    }
    }
  }
}

static void genMem(byte mem[64])
{
  for (uint8_t i = 0; i < 64; i++)
  {
    mem[i] = random(0, 256);
  }
}

static void test()
{
  Serial.println("Entering test mode. Reboot device to exit.");

  clearSerial();
  Serial.print("Enter fast mode? (y/n): ");
  while (!Serial.available());
  int fastMode = Serial.read();
  if(char(fastMode) == 'y')
    setFastMode(true);

  Serial.println("Testing RAM...");
  int i = 0;
  int totalErrors = 0;
  while(true){
    byte mem[64];
    genMem(mem);

    long start = micros();

    for (uint8_t i = 0; i < 64; i++)
    {
      writeData(i, mem[i]);
    }

    uint8_t errors = 0;
    for (uint8_t i = 0; i < 64; i++)
    {
      byte data = readData(i);
      if (data != mem[i])
      {
        errors++;
      }
    }

    long end = micros();
    totalErrors += errors;

    Serial.write(13); // carriage return
    Serial.print("Test ");
    Serial.print(i++);
    Serial.print(", Rate:");
    Serial.print(128 / ((end - start) / 1000000.0));
    Serial.print(" Hz, Errors: ");
    Serial.print(errors);
    Serial.print(" (");
    Serial.print(errors / .64);
    Serial.print("%)");
    Serial.print(", Total errors: ");
    Serial.print(totalErrors);
    Serial.print(" (");
    Serial.print(totalErrors / (i * .64));
    Serial.print("%)");
  }
}