#include "HTU20D.h"

HTU20DSensor::HTU20DSensor(int addr)
{
  if(addr != 0)
  {
    _addr = addr;
  }

  //Check addr
  Wire.beginTransmission(_addr);
  int error = Wire.endTransmission();
  if(error == 0)
  {
    initOK = true;
  }
}

float HTU20DSensor::read()
{
  if(initOK)
  {
    Wire.beginTransmission(_addr);
    //Write E3 - Temp
    Wire.write(0xE3);
    Wire.endTransmission();
    //Read 3Byte
    Wire.requestFrom(_addr, 3);
    if(Wire.available() > 2)
    {
      data = Wire.read()<<8;
      data += Wire.read();
      Wire.read();
    }

    return data;
  }
  return nanf("NA");
}

int HTU20DSensor::read(char* status)
{
  if(initOK)
  {
    char* ptr = status;
    int size = 0;

    Wire.beginTransmission(_addr);
    //Write E3 - Temp
    Wire.write(0xE3);
    Wire.endTransmission();
    //Read 3Byte
    Wire.requestFrom(_addr, 3);
    if(Wire.available() > 2)
    {
      data = Wire.read()<<8;
      data += Wire.read();
      publishData(_addr, "TMP", (float)data, (-46.85+175.72*(float)data/(2l<<15)), "HTU20D");
      Wire.read();
    }

    Wire.beginTransmission(_addr);
    //Write E5 - Humidity
    Wire.write(0xE5);
    Wire.endTransmission();
    //Read 3Byte
    Wire.requestFrom(_addr, 3);
    if(Wire.available() > 2)
    {
      data = Wire.read()<<8;
      data += Wire.read();
      publishData(_addr, "RH", (float)data, (-6+125*(float)data/(2l<<15)), "HTU20D");
      Wire.read();
    }

    return 0;
  }
  return -1;
}
