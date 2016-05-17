#ifndef HTU20D
#define HTU20D

#include "Sensor.h"

#define HTU20DADDRESS 0x40

class HTU20DSensor: public Sensor
{
public:
  int data;
  const enum SensorType sensType = TMP;

  HTU20DSensor(int addr = HTU20DADDRESS);
  float read(void);
  int   read(char*);
};

#endif
