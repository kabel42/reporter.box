#include "Sensor.h"

NULLSensor::NULLSensor(int addr)
{
  _addr = addr;
}

int NULLSensor::read(char* status)
{
  return true;
}

float NULLSensor::read()
{
  return _addr;
}

VCNL4010Sensor::VCNL4010Sensor(int addr): drv()
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

AnalogSensor::AnalogSensor(int addr)
{
  _addr   = addr;
  initOK = true;
}

float AnalogSensor::read()
{
  if(initOK)
  {
    //FIXME: return analogRead(_addr);
  }
  return nanf("NA");
}

int AnalogSensor::read(char* status)
{
  if(initOK)
  {
    //FIXME: sprintf(status, "ANA#%i#?", analogRead(_addr));
    return 0;
  }
  return -1;
}
