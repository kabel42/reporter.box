#include "tinyAudio.h"

AudioSensor::AudioSensor(int addr, int delaytime=1000)
{
  if(addr != 0)
  {
    _addr = addr;
  } else {
    _addr = TINYADDRESS;
  }

  lastDataMillis = millis();
  intervall = delaytime;

  //Check addr
  Wire.beginTransmission(_addr);
  int error = Wire.endTransmission();
  if(error == 0)
  {
    initOK = true;
  }
}

float readEnv()
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

float readGate()
{
  Wire.beginTransmission(_addr);

  Wire.write(0x02);
  Wire.endTransmission();

  Wire.requestFrom(_addr, 2);
  if(Wire.available() > 1)
  {
    data = Wire.read()<<8;
    data += Wire.read();
  }

  return data;
}

float AudioSensor::read()
{
  if(initOK)
  {
    return readEnv();
  }
  return nanf("NA");
}

int AudioSensor::read(char* status)
{
  if(initOK)
  {
    char* ptr = status;
    int size = 0;

    publishData(_addr, "ENV", (float)readEnv(), (float)data, "ENVALOPE");
    publishData(_addr, "GAT", (float)readGate(), (float)data, "GATE");

    return 0;
  }
  return -1;
}

void AudioSensor::poll()
{
  if(initOK)
  {
    Wire.beginTransmission(_addr);

    Wire.write(0x02);
    Wire.endTransmission();

    Wire.requestFrom(_addr, 2);
    if(Wire.available() > 1)
    {
      data = Wire.read()<<8;
      data += Wire.read();

      if(data > 512)
      {
        if(millis() - lastDataMillis > intervall)
        {
          lastDataMillis = millis();

          publishData(_addr, "GAT", (float)data, (float)data, "GATE");

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
        }
      }
    }
  }
}
