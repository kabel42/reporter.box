#ifndef ADC121C
#define ADC121C

#include "Sensor.h"

#define ADC121CADDR 0x50

class ADC121CSensor: public Sensor
{
public:
  int data;
  const enum SensorType sensType = Analog;

  uint8_t _addr = ADC121CADDR;
  ADC121CSensor(int addr=0);
  float read(void);
  int   read(char*);
};

class MQ131Sensor: public ADC121CSensor
{
public:
  uint8_t _addr = 0x50;
  MQ131Sensor(int addr=0):ADC121CSensor(addr){};
  int   read(char*);
};

class MQ4Sensor: public ADC121CSensor
{
public:
  uint8_t _addr = 0x51;
  MQ4Sensor(int addr=0):ADC121CSensor(addr){};
  int   read(char*);
};
#endif
