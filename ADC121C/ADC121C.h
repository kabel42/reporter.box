#ifndef ADC121C
#define ADC121C

#include "Sensor.h"

#define ADC121CADDR 0x50

class ADC121CSensor: public Sensor
{
public:
  int data;
  const enum SensorType sensType = Analog;

  ADC121CSensor(int addr = ADC121CADDR);
  float read(void);
  int   read(char*);
};

#endif
