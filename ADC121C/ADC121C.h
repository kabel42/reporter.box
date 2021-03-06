#ifndef ADC121C
#define ADC121C

#include "Sensor.h"

#define ADC121CADDR 0x50

class ADC121CSensor: public Sensor
{
public:
  int data;
  const enum SensorType sensType = Analog;
  float offset = 0;
  float scale  = 0;

  ADC121CSensor(int addr=0);
  float read(void);
  int   read(char*);
  bool  getCal(char*);
  bool setCal(char*, float, float);
};

class MQ131Sensor: public ADC121CSensor
{
public:
  MQ131Sensor(int addr=0):ADC121CSensor(addr)
  {
    if(addr == 0)
    {
      _addr = 0x50;
    }
  };
  int read(char*);
  bool getCal(char*);
  bool setCal(char*, float, float);
};

class MQ4Sensor: public ADC121CSensor
{
public:
  MQ4Sensor(int addr=0):ADC121CSensor(addr)
  {
    if(addr == 0)
    {
      _addr = 0x51;
    }
  };
  int read(char*);
  bool getCal(char*);
  bool setCal(char*, float, float);
};

class MQ135Sensor: public ADC121CSensor
{
public:
  MQ135Sensor(int addr=0):ADC121CSensor(addr)
  {
    if(addr == 0)
    {
      _addr = 0x52;
    }
  };
  int read(char*);
  bool getCal(char*);
  bool setCal(char*, float, float);
};
#endif
