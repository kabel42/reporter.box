#include <math.h>
#include <list>

#include "Sensor.h"
#include "HTU20D.h"
#include "ADC121C.h"

Sensor *S1; //I2C 1
Sensor *S2; //I2C 2
Sensor *S3; //I2C 3
Sensor *S4; //I2C 4

char data[255];
int delaytime = 60000;

void setup()
{
  //Setup I2C
  Wire.setSpeed(CLOCK_SPEED_100KHZ);
  Wire.begin();

  for(uint8_t i=0; i<128; i++)
  {
    Wire.beginTransmission(i);
    int error = Wire.endTransmission();

    if(error == 0)
    {
      snprintf(data, 250, "0x%X", i);
      Particle.publish("I2C", data);
      delay(1000);
    }
  }

  static HTU20DSensor S1tmp(0);
  S1 = &S1tmp;

  static VCNL4010Sensor S2tmp(0);
  S2 = &S2tmp;

  static ADC121CSensor S3tmp(0x50);
  S3 = &S3tmp;

  static ADC121CSensor S4tmp(0x51);
  S4 = &S4tmp;

  pinMode(D7, OUTPUT);
}

void loop()
{
  if(S1->initOK)
  {
    int error = S1->read(data);
  }

  //S2
  if(S2->initOK)
  {
    int error = S2->read(data);
  }

  //S3
  if(S3->initOK)
  {
    int error = S3->read(data);
  }

  //S4
  if(S4->initOK)
  {
    int error = S4->read(data);
  }

  {
    static uint8_t i = 0;
    analogWrite(D7, i++);
  }

  delay(delaytime);
}
