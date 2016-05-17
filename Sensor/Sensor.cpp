#include "Sensor.h"

NULLSensor::NULLSensor(int pin)
{
  val = pin;
}

int NULLSensor::read(char* status)
{
  return true;
}

float NULLSensor::read()
{
  return val;
}

VCNL4010Sensor::VCNL4010Sensor(int nc): drv()
{
  initOK = (drv.begin());
}

float VCNL4010Sensor::read()
{
  if(initOK)
  {
    //return drv.readAmbient();
    return drv.readAmbient();
  }
  return nanf("NA");
}

int VCNL4010Sensor::read(char* status)
{
  if(initOK)
  {
    sprintf(status, "AMB#%i#?##DIST#%i#?", drv.readAmbient(), drv.readProximity());
    return 0;
  }
  return -1;
}

AnalogSensor::AnalogSensor(int pin)
{
  _pin   = pin;
  initOK = true;
}

float AnalogSensor::read()
{
  if(initOK)
  {
    return analogRead(_pin);
  }
  return nanf("NA");
}

int AnalogSensor::read(char* status)
{
  if(initOK)
  {
    sprintf(status, "ANA#%i#?", analogRead(_pin));
    return 0;
  }
  return -1;
}
