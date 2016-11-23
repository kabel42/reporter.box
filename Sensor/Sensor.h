#ifndef SENSOR_H
#define SENSOR_H

#include <math.h>

#include "application.h"

#include "VCNL4010.h"
#include "Adafruit_AM2315.h"
#include "SparkFunISL29125.h"

#define CALTIME 60000

enum SensorType
{
  None = 0,
  Light,    //VCNL4010
  Analog,
  TMP,
  Color,
  Audio,
  Soil,
};

class Sensor {
public:
  uint8_t _addr = 0;
  bool initOK = false;
  const enum SensorType sensType = None;

  virtual float read(void)    = 0;
  virtual int   read(char*)   = 0;
  virtual bool  getCal(char*) = 0;
  virtual float getVal(char*);
  virtual bool  setCal(char*, float, float) = 0;
};

void publishData(uint8_t addr, char* measID, float raw, float offset, float scale, char* sens);
void publishCal(char* measId, float min, float max, float offset, float scale);

bool calLoop(Sensor *S, char *id, float *min, float *max);

class NULLSensor: public Sensor
{
public:
  const enum SensorType sensType = None;
  float val;

  NULLSensor(int addr=0);
  float read(void);
  int   read(char*);
  bool  getCal(char*);
  float getVal(char*);
  bool  setCal(char*, float, float);
};

class VCNL4010Sensor: public Sensor
{
public:
  VCNL4010 drv;
  const enum SensorType sensType = Light;
  float offsetAMB = 0;
  float scaleAMB  = 0;
  float offsetPRX = 0;
  float scalePRX  = 0;

  VCNL4010Sensor(int addr=0);
  float read(void);
  int   read(char*);
  bool  getCal(char*);
  float getVal(char*);
  bool  setCal(char*, float, float);
};

class AnalogSensor: public Sensor
{
public:
  const enum SensorType sensType = Analog;
  float offset = 0;
  float scale  = 0;

  AnalogSensor(int addr=0);
  float read(void);
  int   read(char*);
  bool  getCal(char*);
  bool  setCal(char*, float, float);
};

class AM2315Sensor: public Sensor
{
public:
  Adafruit_AM2315 drv;
  const enum SensorType sensType = Light;
  float offsetTMP = 0;
  float scaleTMP  = 0;
  float offsetRH = 0;
  float scaleRH  = 0;

  AM2315Sensor(int addr=0);
  float read(void);
  int   read(char*);
  bool  getCal(char*);
  float getVal(char*);
  bool  setCal(char*, float, float);
};

class ISL29125Sensor: public Sensor
{
public:
  SFE_ISL29125 drv;
  const enum SensorType sensType = Color;
  float offsetR = 0;
  float scaleR  = 0;
  float offsetG = 0;
  float scaleG  = 0;
  float offsetB = 0;
  float scaleB  = 0;

  ISL29125Sensor(int addr=0);
  float read(void);
  int   read(char*);
  bool  getCal(char*);
  float getVal(char*);
  bool  setCal(char*, float, float);
};
#endif
