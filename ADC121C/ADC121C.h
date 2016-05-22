#ifndef ADC121C
#define ADC121C

#include "Sensor.h"

#define ADC121CADDR 0x50

class ADC121CSensor: public Sensor
{
public:
  uint8_t _addr = ADC121CADDR;
  int data;
  const enum SensorType sensType = Analog;

  ADC121CSensor(int addr=0);
  float read(void);
  int   read(char*);
};

class MQ131Sensor: public ADC121CSensor
{
public:
  uint8_t _addr = 0x50;
  MQ131Sensor(int addr=0):ADC121CSensor(addr){};
};

class MQ4Sensor: public ADC121CSensor
{
public:
  uint8_t _addr = 0x51;
  MQ4Sensor(int addr=0):ADC121CSensor(addr){};
};
#endif
