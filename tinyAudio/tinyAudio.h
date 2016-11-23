#ifndef TINYAUDIO
#define TINYAUDIO

#include "Sensor.h"

#define TINYADDRESS 0x08

class AudioSensor: public Sensor
{
public:
  const enum SensorType sensType = Audio;
  int data;
  unsigned long lastDataMillis;
  int intervall;
  float offset = 0;
  float scale  = 0;

  AudioSensor(int addr);
  AudioSensor(int addr, int delaytime);
  float read(void);
  int   read(char*);
  void  poll();
  bool  getCal(char*);
  bool setCal(char*, float, float);
};
#endif
