#include "SoilSens.h"

SoilSensor::SoilSensor(int addr)
{
  if(addr != 0)
  {
    _addr = addr;
  } else {
    _addr = SOILADDRESS;
  }

  //Check addr
  Wire.beginTransmission(_addr);
  int error = Wire.endTransmission();
  if(error == 0)
  {
    initOK = true;
  }
}

float SoilSensor::read()
{
  if(initOK)
  {
    Wire.beginTransmission(_addr);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(_addr, (uint8_t)2);
    if(Wire.available() > 1)
    {
      data = Wire.read()<<8;
      data += Wire.read();
    }

    return data;
  }
  return nanf("NA");
}

int SoilSensor::read(char* status)
{
  if(initOK)
  {
    char* ptr = status;
    int size = 0;

    data = read();
    publishData(_addr, "SOI", (float)data, offset, scale, "SOILCAP");
    return 0;
  }
  return -1;
}

bool SoilSensor::getCal(char *id)
{
  return calLoop(this, id, &offset, &scale);
}
