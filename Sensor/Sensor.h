#ifndef SENSOR_H
#define SENSOR_H

#include <math.h>

#include "application.h"

#include "VCNL4010.h"
#include "Adafruit_AM2315.h"
#include "SparkFunISL29125.h"

enum SensorType
{
  None = 0,
  Light,    //VCNL4010
  Analog,
  TMP,
  Color,
};

void publishData(uint8_t addr, char* measID, float raw, float data, char* sens);

class Sensor {
public:
  uint8_t _addr = 0;
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

  NULLSensor(int addr=0);
  float read(void);
  int   read(char*);
};

class VCNL4010Sensor: public Sensor
{
public:
  VCNL4010 drv;
  const enum SensorType sensType = Light;

  VCNL4010Sensor(int addr=0);
  float read(void);
  int   read(char*);
};

class AnalogSensor: public Sensor
{
public:
  const enum SensorType sensType = Analog;

  AnalogSensor(int addr=0);
  float read(void);
  int   read(char*);
};

class AM2315Sensor: public Sensor
{
public:
  Adafruit_AM2315 drv;
  const enum SensorType sensType = Light;

  AM2315Sensor(int addr=0);
  float read(void);
  int   read(char*);
};

class ISL29125Sensor: public Sensor
{
public:
  SFE_ISL29125 drv;
  const enum SensorType sensType = Color;

  ISL29125Sensor(int addr=0);
  float read(void);
  int   read(char*);
};
#endif
