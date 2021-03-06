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

int readTemp(uint8_t _addr)
{
  int data = nanf("NA");
  Wire.beginTransmission(_addr);
  //Write E3 - Temp
  Wire.write(0xE3);
  Wire.endTransmission();
  //Read 3Byte
  Wire.requestFrom(_addr, (uint8_t)3);
  if(Wire.available() > 2)
  {
    data = Wire.read()<<8;
    data += Wire.read();
    Wire.read();
  }
  return data;
}

int readHum(uint8_t _addr)
{
  int data = nanf("NA");
  Wire.beginTransmission(_addr);
  //Write E5 - Humidity
  Wire.write(0xE5);
  Wire.endTransmission();
  //Read 3Byte
  Wire.requestFrom(_addr, (uint8_t)3);
  if(Wire.available() > 2)
  {
    data = Wire.read()<<8;
    data += Wire.read();
    Wire.read();
  }
  return data;
}

float HTU20DSensor::read()
{
  if(initOK)
  {
    return readTemp(_addr);
  }
  return nanf("NA");
}

int HTU20DSensor::read(char* status)
{
  if(initOK)
  {
    char* ptr = status;
    int size = 0;

    data = readTemp(_addr);
    publishData(_addr, "TMP", (-46.85+175.72*(float)data/(2l<<15)), offsetTMP, scaleTMP, "HTU20D");

    data = readHum(_addr);
    publishData(_addr, "RH", (-6+125*(float)data/(2l<<15)), offsetRH, scaleRH, "HTU20D");

    return 0;
  }
  return -1;
}

float HTU20DSensor::getVal(char *id)
{
  if(id[1] == 'R' && id[2] == 'H') {
    return readHum(_addr);
  } else if(id[1] == 'T' && id[2] == 'M' && id[3] == 'P') {
    return readTemp(_addr);
  } else {
    return nanf("NA");
  }
}

bool HTU20DSensor::getCal(char *id)
{
  if(id[1] == 'R' && id[2] == 'H') {
    return calLoop(this, id, &offsetRH, &scaleRH);
  } else if(id[1] == 'T' && id[2] == 'M' && id[3] == 'P') {
    return calLoop(this, id, &offsetTMP, &scaleTMP);
  } else {
    return false;
  }
}

bool HTU20DSensor::setCal(char* id, float offset, float scale)
{
  if(id[1] == 'R' && id[2] == 'H') {
    offsetRH = offset;
    scaleRH  = scale;
    return true;
  } else if(id[1] == 'T' && id[2] == 'M' && id[3] == 'P') {
    offsetTMP = offset;
    scaleTMP  = scale;
    return true;
  } else {
    return false;
  }
}
