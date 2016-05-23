#include "ADC121C.h"

ADC121CSensor::ADC121CSensor(int addr)
{
  if(addr != 0)
  {
    _addr = addr;
  } else {
    _addr = ADC121CADDR;
  }

  //Check addr
  Wire.beginTransmission(_addr);
  int error = Wire.endTransmission();
  if(error == 0)
  {
    initOK = true;
  }
}

float ADC121CSensor::read()
{
  if(initOK)
  {
    Wire.beginTransmission(_addr);

    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(_addr, 2);
    if(Wire.available() == 2)
    {
      data = (Wire.read()&0x0F)<<8;
      data += Wire.read();
    }

    return data;
  }
  return nanf("NA");
}

int ADC121CSensor::read(char* status)
{
  if(initOK)
  {
    data = read();

    publishData(_addr, "ADC", data, ((9.95 / 4096.0) * data + 0.05), "AD121C");

    return 0;
  }
  return -1;
}

int MQ131Sensor::read(char* status)
{
  if(initOK)
  {
    data = ADC121CSensor::read();

    publishData(_addr, "OZO", data, ((100 / 4096.0) * data), "MQ131");

    return 0;
  }
  return -1;
}

int MQ4Sensor::read(char* status)
{
  if(initOK)
  {
    data = ADC121CSensor::read();

    publishData(_addr, "MET", data, ((100 / 4096.0) * data), "MQ4");

    return 0;
  }
  return -1;
}

int MQ135Sensor::read(char* status)
{
  if(initOK)
  {
    data = ADC121CSensor::read();

    publishData(_addr, "NH3", data, ((100 / 4096.0) * data), "MQ135");

    return 0;
  }
  return -1;
}
