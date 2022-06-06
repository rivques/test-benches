// ---------------------------------------------------------------- /
// Arduino I2C LCD Scanner
// Based on code by Arbi Abdul Jabbaar at https://create.arduino.cc/projecthub/abdularbi17/how-to-scan-i2c-address-in-arduino-eaadda
// ---------------------------------------------------------------- /

#include <Wire.h> //include Wire.h library
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define SERVO_PIN 2
#define TRIG_PIN 10
#define ECHO_PIN 11

#define SERVO_TEST_LENGTH 4000 // how long the servo test should last

Servo testServo;
LiquidCrystal_I2C lcd(0x00, 16, 2);

long startMillis; // helps the servo test take a consistent amount of time

void setup()
{
  Wire.begin(); // Wire communication begin
  Serial.begin(9600); // The baudrate of Serial monitor is set in 9600
  while (!Serial); // Waiting for Serial Monitor
  Serial.println("\nI2C Scanner");
  testServo.attach(SERVO_PIN);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop()
{
  byte error, address; //variable for error and I2C address
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
      lcd = LiquidCrystal_I2C(address, 16, 2); // initialize the lcd at the address
      lcd.begin(16, 2);
      lcd.backlight();
      lcd.print("0x"); // write the address to the lcd
      if (address < 16)
        lcd.print("0");
      lcd.print(address, HEX);
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  startMillis = millis(); // for test timing

  while(millis() - startMillis < SERVO_TEST_LENGTH/2) { // the first sweep
    int servoVal = map(millis()-startMillis, 0, SERVO_TEST_LENGTH/2, 0, 180);
    testServo.write(servoVal);
    // ultrasonic test
    lcd.setCursor(0, 1);
    lcd.print(getDistance());
    lcd.print("                "); // clear the rest of the line
    Serial.print(servoVal); // some debug
    Serial.print(", time: ");
    Serial.println(millis()-startMillis);
  }
  // same thing but the servo goes the other way
  while(millis() - startMillis < SERVO_TEST_LENGTH) {
    int servoVal = map(millis()-startMillis, SERVO_TEST_LENGTH/2, SERVO_TEST_LENGTH, 180, 0);
    testServo.write(servoVal);
    // ultrasonic test
    lcd.setCursor(0, 1);
    lcd.print(getDistance());
    lcd.print("                "); // clear the rest of the line
    Serial.print(servoVal); // some debug
    Serial.print(", time: ");
    Serial.println(millis()-startMillis);
  }
}

long getDistance(){
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // outward pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // receive pulse
  long duration = pulseIn(ECHO_PIN, HIGH);
  // Calculate the distance
  long distance = duration * 0.034 / 2; 
  return distance;
}
