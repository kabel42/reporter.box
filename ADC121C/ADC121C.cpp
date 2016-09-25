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

    Wire.requestFrom(_addr, (uint8_t)2);
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

    publishData(_addr, "ADC", ((9.95 / 4096.0) * data + 0.05), offset, scale, "AD121C");

    return 0;
  }
  return -1;
}

bool ADC121CSensor::getCal(char *id)
{
  return calLoop(this, id, &offset, &scale);
}

int MQ131Sensor::read(char* status)
{
  if(initOK)
  {
    data = ADC121CSensor::read();

    publishData(_addr, "OZO", ((100 / 4096.0) * data), offset, scale, "MQ131");

    return 0;
  }
  return -1;
}

bool MQ131Sensor::getCal(char *id)
{
  return calLoop(this, id, &offset, &scale);
}

int MQ4Sensor::read(char* status)
{
  if(initOK)
  {
    data = ADC121CSensor::read();

    publishData(_addr, "MET", ((100 / 4096.0) * data), offset, scale, "MQ4");

    return 0;
  }
  return -1;
}

bool MQ4Sensor::getCal(char *id)
{
  return calLoop(this, id, &offset, &scale);
}

int MQ135Sensor::read(char* status)
{
  if(initOK)
  {
    data = ADC121CSensor::read();

    publishData(_addr, "NH3", ((100 / 4096.0) * data), offset, scale, "MQ135");

    return 0;
  }
  return -1;
}

bool MQ135Sensor::getCal(char *id)
{
  return calLoop(this, id, &offset, &scale);
}
