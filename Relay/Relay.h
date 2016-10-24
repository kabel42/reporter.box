#ifndef RELAY_H
#define RELAY_H
#define RELAY_I2C_ADDR 0x20

#include "application.h"

class Relay {
  const int relayPins[4] = {D3, D4, D5, D6};
  uint8_t relay_addr;
  uint8_t status;
public:
  bool i2c;
  bool initOK;
  Relay(bool a0=false, bool a1=false, bool a2=false);
  int setRelay(long num, bool state);
  int activateRelay(String data);
  int deactivateRelay(String data);
  int getStatus(String data);
  bool getRelay(int num);
};
#endif
