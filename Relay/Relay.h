#ifndef RELAY_H
#define RELAY_H
#define RELAY_I2C_ADDR 0x20

#include "application.h"

class Relay {
  const int relayPins[4] = {D3, D4, D5, D6};
public:
  bool i2c;
  Relay();
  int setRelay(long num, bool state);
  int activateRelay(String data);
  int deactivateRelay(String data);
  bool getRelay(int num);
};
#endif
