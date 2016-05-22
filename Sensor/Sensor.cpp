#include "Sensor.h"

void publishData(uint8_t addr, char* measID, float raw, float data, char* sens)
{
  static char pubstring[252];
  snprintf(pubstring, 250, "{\"RawValue\": %f, \"CalValue\": %f, \"measurementID\": \"%s\", \"addr\": %d}, ", raw, data, measID, addr);

  Particle.publish(sens, pubstring);
  delay(1000);
}

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
  if(addr != 0)
  {
    _addr = addr;
  } else {
    _addr = VCNL4010_ADRESS;
  }

  //Check addr
  Wire.beginTransmission(_addr);
  int error = Wire.endTransmission();

  if(error == 0)
  {
    initOK = (drv.begin(_addr));
  }
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
    //sprintf(status, "AMB#%i#?##DIST#%i#?", drv.readAmbient(), drv.readProximity());
    publishData(VCNL4010_ADRESS, "AMB", drv.readAmbient(), nanf("TBD"), "VCNL4010");
    publishData(VCNL4010_ADRESS, "PRX", drv.readProximity(), nanf("TBD"), "VCNL4010");
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
