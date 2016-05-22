#ifndef SENSOR_H
#define SENSOR_H

#include <math.h>

#include "application.h"

#include "VCNL4010.h"

enum SensorType
{
  None = 0,
  Light,    //VCNL4010
  Analog,
  TMP,
};

void publishData(uint8_t addr, char* measID, float raw, float data, char* sens);

class Sensor {
public:
  uint8_t _addr;
  _Bool initOK = false;
  const enum SensorType sensType = None;

  virtual float read(void)  = 0;
  virtual int   read(char*) = 0;
};

class NULLSensor: public Sensor
{
public:
  const enum SensorType sensType = None;
  float val;

  NULLSensor(int addr);
  float read(void);
  int   read(char*);
};

class VCNL4010Sensor: public Sensor
{
public:
  VCNL4010 drv;
  const enum SensorType sensType = Light;

  VCNL4010Sensor(int addr);
  float read(void);
  int   read(char*);
};

class AnalogSensor: public Sensor
{
public:
  const enum SensorType sensType = Analog;

  AnalogSensor(int addr);
  float read(void);
  int   read(char*);
};

#endif
