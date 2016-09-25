#include "Relay.h"

Relay::Relay() {
  //Check addr
  Wire.beginTransmission(RELAY_I2C_ADDR);
  int error = Wire.endTransmission();

  if(error == 0)
  {
    i2c = true;
  } else {
    i2c = false;
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
  }

  Particle.function("activate",   &Relay::activateRelay,   this);
  Particle.function("deactivate", &Relay::deactivateRelay, this);
}

int Relay::setRelay(long num, bool newState)
{
  if (num > 0 && num < 5) {
    if(i2c)
    {
      //TODO
      return -1;
    } else {
      digitalWrite(relayPins[num-1], newState);
    }
    return num;
  }
  return 0;
}

int Relay::activateRelay(String data)
{
    return setRelay(data.toInt(), true);
}

int Relay::deactivateRelay(String data)
{
    return setRelay(data.toInt(), false);
}

bool Relay::getRelay(int num)
{
  if (num > 0 && num < 5) {
    if(i2c)
    {
      //TODO
      return false; //TODO
    } else {
      digitalRead(relayPins[num-1]);
    }
  }
}
