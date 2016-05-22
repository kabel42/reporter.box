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
    //Write E3 - Temp
    Wire.write(0x00);
    Wire.endTransmission();
    //Read 3Byte
    Wire.requestFrom(_addr, 2);
    if(Wire.available() == 2)
    {
      data = (Wire.read()&0x0F)<<8;
      data += Wire.read();
    }

    return (9.95 / 4096.0) * data + 0.05;
  }
  return nanf("NA");
}

int ADC121CSensor::read(char* status)
{
  if(initOK)
  {
    char* ptr = status;
    int size = 0;

    Wire.beginTransmission(_addr);
    //Write E3 - Temp
    Wire.write(0x00);
    Wire.endTransmission();
    //Read 3Byte
    Wire.requestFrom(_addr, 2);
    if(Wire.available() == 2)
    {
      data = (Wire.read()&0x0F)<<8;
      data += Wire.read();
    }

    publishData(_addr, "ADC", data, ((9.95 / 4096.0) * data + 0.05), "ADC121C");

    return 0;
  }
  return -1;
}
