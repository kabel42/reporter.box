#include "Sensor.h"

void publishData(uint8_t addr, char* measID, float raw, float data, char* sens)
{
  static char pubstring[252];
  //snprintf(pubstring, 250, "{\"uncalibrated_value\": %f, \"calibrated_value\": %f, \"Measurement ID\": \"%s\", \"sensor\": {\"address:\"%d, \"name\":%s}}, ", raw, data, measID, addr, sens);

  //Gen Id from addr and measID
  uint32_t id = addr<<(3*8);
  int len = strlen(measID);
  if(len>3)
    len = 3;

  for(int i = 0; i<len; i++)
  {
    id += measID[i]<<(8*(2-i));
  }

  char rawStr[35];
  char dataStr[35];

  if(isfinite(raw))
  {
    snprintf(rawStr, 34, "\"uncalibrated_value\": %.2f,", raw);
  } else {
    snprintf(rawStr, 34, "");
  }

  if(isfinite(data))
  {
    snprintf(dataStr, 34, "\"calibrated_value\": %.2f,", data);
  } else {
    snprintf(dataStr, 34, "");
  }
  snprintf(pubstring, 250, "{%s %s \"sensor\": {\"address\": %li}}", rawStr, dataStr, id);

  Particle.publish("measurement", pubstring);
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

AM2315Sensor::AM2315Sensor(int addr): drv()
{
  if(addr != 0)
  {
    _addr = addr;
  } else {
    _addr = AM2315_I2CADDR;
  }

  //Check addr
  Wire.beginTransmission(_addr);
  int error = Wire.endTransmission();

  if(error == 0)
  {
    initOK = (drv.begin());
  }
}

float AM2315Sensor::read()
{
  if(initOK)
  {
    //return drv.readAmbient();
    return drv.readTemperature();
  }
  return nanf("NA");
}

int AM2315Sensor::read(char* status)
{
  if(initOK)
  {
    //sprintf(status, "AMB#%i#?##DIST#%i#?", drv.readAmbient(), drv.readProximity());
    publishData(_addr, "TMP", drv.readTemperature(), nanf("TBD"), "AM2315");
    publishData(_addr, "RH", drv.readHumidity(), nanf("TBD"), "AM2315");
    return 0;
  }
  return -1;
}

ISL29125Sensor::ISL29125Sensor(int addr): drv()
{
  if(addr != 0)
  {
    _addr = addr;
  } else {
    _addr = ISL_I2C_ADDR;
  }

  //Check addr
  Wire.beginTransmission(_addr);
  int error = Wire.endTransmission();

  if(error == 0)
  {
    initOK = (drv.init());
  }
}

float ISL29125Sensor::read()
{
  if(initOK)
  {
    //return drv.readAmbient();
    return drv.readRed()+drv.readBlue()+drv.readGreen();
  }
  return nanf("NA");
}

int ISL29125Sensor::read(char* status)
{
  if(initOK)
  {
    float red   = drv.readRed();
    float green = drv.readGreen();
    float blue  = drv.readBlue();
    float mul   = 10./65535.;
    publishData(_addr, "Red", red, red*mul, "ISE29125");
    publishData(_addr, "Green", green, green*mul, "ISE29125");
    publishData(_addr, "Blue", blue, blue*mul, "ISE29125");
    return 0;
  }
  return -1;
}
