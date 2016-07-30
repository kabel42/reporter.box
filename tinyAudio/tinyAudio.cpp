#include "tinyAudio.h"

AudioSensor::AudioSensor(int addr)
{
  if(addr != 0)
  {
    _addr = addr;
  } else {
    _addr = TINYADDRESS;
  }

  //Check addr
  Wire.beginTransmission(_addr);
  int error = Wire.endTransmission();
  if(error == 0)
  {
    initOK = true;
  }
}

float AudioSensor::read()
{
  if(initOK)
  {
    Wire.beginTransmission(_addr);
    Wire.write(0x01);
    Wire.endTransmission();

    Wire.requestFrom(_addr, 2);
    if(Wire.available() > 1)
    {
      data = Wire.read()<<8;
      data += Wire.read();
    }

    return data;
  }
  return nanf("NA");
}

int AudioSensor::read(char* status)
{
  if(initOK)
  {
    char* ptr = status;
    int size = 0;

    Wire.beginTransmission(_addr);

    Wire.write(0x01);
    Wire.endTransmission();

    Wire.requestFrom(_addr, 2);
    if(Wire.available() > 1)
    {
      data = Wire.read()<<8;
      data += Wire.read();
      publishData(_addr, "ENV", (float)data, (float)data, "ENVALOPE");
    }

    Wire.beginTransmission(_addr);

    Wire.write(0x02);
    Wire.endTransmission();

    Wire.requestFrom(_addr, 2);
    if(Wire.available() > 1)
    {
      data = Wire.read()<<8;
      data += Wire.read();
      publishData(_addr, "GAT", (float)data, (float)data, "GATE");
    }

    return 0;
  }
  return -1;
}
