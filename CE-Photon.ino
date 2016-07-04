#include <math.h>
#include <algorithm>    // std::for_each
#include <vector>       // std::vector

#include "Sensor.h"
#include "HTU20D.h"
#include "ADC121C.h"

std::vector<Sensor*> sensorList;

char data[255];
int delaytime = 60000;

int readIfInit(Sensor *S);

int readIfInit(Sensor *S)
{
  if(S)
  {
    if(S->initOK)
    {
      return S->read(data);
    }
  }
  delay(1000);
  return 0;
}

void setup()
{
  //Setup I2C
  Wire.setSpeed(10000);
  Wire.begin();

  //Init Sensors
  sensorList.push_back(new AM2315Sensor(0));
  sensorList.push_back(new HTU20DSensor(0));
  sensorList.push_back(new VCNL4010Sensor(0));
  sensorList.push_back(new ISL29125Sensor(0));
  sensorList.push_back(new MQ131Sensor(0x50));
  sensorList.push_back(new MQ4Sensor(0x51));
  sensorList.push_back(new MQ135Sensor(0x52));

  pinMode(D7, OUTPUT);
}

void loop()
{
  for(Sensor *S: sensorList)
  {
    readIfInit(S);
  }

  static uint8_t i = 0;
  analogWrite(D7, i++);

  delay(delaytime);
}
