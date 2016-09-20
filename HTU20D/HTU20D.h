#ifndef HTU20D
#define HTU20D

#include "Sensor.h"

#define HTU20DADDRESS 0x40

class HTU20DSensor: public Sensor
{
public:
  int data;
  uint8_t _addr = HTU20DADDRESS;
  const enum SensorType sensType = TMP;

  HTU20DSensor(int addr=0);
  float read(void);
  int   read(char*);
  bool  getCal(char*);
};

#endif
