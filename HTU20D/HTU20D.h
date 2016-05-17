#ifndef HTU20D
#define HTU20D

#include "Sensor.h"

class HTU20DSensor: public Sensor
{
public:
  const int _addr = 0x40;
  int data;
  const enum SensorType sensType = TMP;

  HTU20DSensor(int addr);
  float read(void);
  int   read(char*);
};

#endif
