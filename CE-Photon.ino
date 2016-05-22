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

  //FIX AM2315
  Wire.beginTransmission(AM2315_I2CADDR);
  Wire.endTransmission();
  delay(500);
  //End FIX

  for(uint8_t i=1; i<128; i++)
  {
    Wire.beginTransmission(i);
    delay(10);
    int error = Wire.endTransmission();
    delay(10);

    if(error == 0)
    {
      snprintf(data, 250, "0x%X", i);
      Particle.publish("I2C", data);
      delay(1000);
    }
  }

  Particle.publish("I2C", "Done");

  static AM2315Sensor S1tmp(0);
  S1 = &S1tmp;

  static VCNL4010Sensor S2tmp(0);
  S2 = &S2tmp;

  static MQ131Sensor S3tmp(0);
  S3 = &S3tmp;

  static MQ4Sensor S4tmp(0);
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
