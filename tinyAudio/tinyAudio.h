#ifndef TINYAUDIO
#define TINYAUDIO

#include "Sensor.h"

#define TINYADDRESS 0x08

class AudioSensor: public Sensor
{
public:
  const enum SensorType sensType = Audio;
  int data;

  AudioSensor(int addr=0);
  float read(void);
  int   read(char*);
};
#endif
