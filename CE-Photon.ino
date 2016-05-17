#include <math.h>

#include "Sensor.h"

Sensor *S1; //I2C 1
Sensor *S2; //I2C 2
Sensor *S3; //I2C 3
Sensor *S4; //I2C 4

void setup()
{
  //Setup I2C
  Wire.setSpeed(CLOCK_SPEED_100KHZ);
  Wire.begin();

  static NULLSensor S1tmp(0);
  S1 = &S1tmp;

  static NULLSensor S2tmp(0);
  S2 = &S2tmp;

  static NULLSensor S3tmp(0);
  S3 = &S3tmp;

  static NULLSensor S4tmp(0);
  S4 = &S4tmp;

  pinMode(D7, OUTPUT);
}

int data;
char dataStr[250];

void loop()
{
  if(S1->initOK)
  {
    int error = S1->read("");
  }

  //S2
  if(S2->initOK)
  {
    int error = S2->read("");
  }

  //S3
  if(S3->initOK)
  {
    int error = S3->read("");
  }

  //S4
  if(S4->initOK)
  {
    int error = S4->read("");
  }

  //Test...
  const uint8_t HTU_Addr = 0x40;
  Wire.beginTransmission(HTU_Addr);
  //Write E3 - Temp
  Wire.write(0xE3);
  Wire.endTransmission();
  //Read 3Byte
  Wire.requestFrom(HTU_Addr, 3);
  if(Wire.available() > 2)
  {
    data = Wire.read()<<8;
    data += Wire.read();
    sprintf(dataStr, "{Temp:%f}", -46.85+175.72*(float)data/(2l<<15));
    Particle.publish("TMP", dataStr);
    Wire.read();
  }

  Wire.beginTransmission(HTU_Addr);
  //Write E5 - Humidity
  Wire.write(0xE5);
  Wire.endTransmission();
  //Read 3Byte
  Wire.requestFrom(HTU_Addr, 3);
  if(Wire.available() > 2)
  {
    data = Wire.read()<<8;
    data += Wire.read();
    sprintf(dataStr, "{RH:%f}", -6+125*(float)data/(2l<<15));
    Particle.publish("RH", dataStr);
    Wire.read();
  }

  {
    static uint8_t i = 0;
    analogWrite(D7, i++);
  }
  delay(10000);
}
