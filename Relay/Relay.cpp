#include "Relay.h"

Relay::Relay(bool a0, bool a1, bool a2) {
  //Check addr
  relay_addr = RELAY_I2C_ADDR | (a0 ? 1 : 0) | (a1 ? 2 : 0) | (a2 ? 4 : 0);
  Wire.beginTransmission(relay_addr);
  int error = Wire.endTransmission();

  if(error == 0)
  {
    i2c = true;
    initOK = true;

    Wire.beginTransmission(relay_addr);
    Wire.write(0x00); //IO Direction
    Wire.write(0xF0); //Set relay as output (IO3-IO0)
    int error = Wire.endTransmission();
    if(error)
    {
      initOK = false;
      Particle.publish("error", "relay i2c init failed");
    }
  } else {
    i2c = false;
    initOK = true;
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
  }

  Particle.function("activate",   &Relay::activateRelay,   this);
  Particle.function("deactivate", &Relay::deactivateRelay, this);
  Particle.function("getStatus", &Relay::getStatus, this);
}

int Relay::setRelay(long num, bool newState)
{
  if (num > 0 && num < 5) {
    if(i2c)
    {
      static uint8_t relayStatus = 0;
      if(newState)
      {
        relayStatus |= 1<<(num-1);
      } else {
        relayStatus &= ~(1<<(num-1));
      }

      Wire.beginTransmission(relay_addr);
      Wire.write(0x09); //GPIO PORT
      Wire.write(relayStatus); //Set relay status (IO3-IO0)
      int error = Wire.endTransmission();

      //TODO handle error?

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

int Relay::getStatus(String data)
{
  if(i2c)
  {
    Wire.beginTransmission(relay_addr);
    Wire.write(0x09); //GPIO PORT
    Wire.endTransmission();

    Wire.requestFrom(relay_addr, (uint8_t)1);
    if(Wire.available())
    {
      status = Wire.read() & 0x0F;
    } else {
      return 0;
    }
  } else {
    status = 0;
    for(uint8_t i = 0; i<4; i++)
    {
      status |= digitalRead(relayPins[i])<<i;
    }
  }
  return (int)status;
}

bool Relay::getRelay(int num)
{
  if (num > 0 && num < 5) {
    if(i2c)
    {
      Wire.beginTransmission(relay_addr);
      Wire.write(0x09); //GPIO PORT
      Wire.endTransmission();

      Wire.requestFrom(relay_addr, (uint8_t)1);
      if(Wire.available())
      {
        status = Wire.read();
        return ((status>>(num-1))&1);
      }
      return false;
    } else {
      digitalRead(relayPins[num-1]);
    }
  }
}
