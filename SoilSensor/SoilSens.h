#ifndef SOILSENS
#define SOILSENS

#include "Sensor.h"

#define SOILADDRESS 0x30

class SoilSensor: public Sensor
{
public:
  const enum SensorType sensType = Soil;
  int data;

  SoilSensor(int addr=0);
  float read(void);
  int   read(char*);
};
#endif
