#ifndef _VCNL4010_H_
#define _VCNL4010_H_

#include "application.h"
#include "stdlib.h"

#include "spark_wiring.h"
#include "spark_wiring_i2c.h"

#define VCNL4010_DEBUG 0

// the i2c address
#define VCNL4010_ADRESS 0x13 //0b0010011

// commands and constants
#define VCNL4010_COMMAND 0x80
#define VCNL4010_PRODUCTID 0x81
#define VCNL4010_PROXRATE 0x82
#define VCNL4010_IRLED 0x83
#define VCNL4010_AMBIENTPARAMETER 0x84
#define VCNL4010_AMBIENTDATA 0x85
#define VCNL4010_PROXIMITYDATA 0x87
#define VCNL4010_INTCONTROL 0x89
#define VCNL4010_PROXINITYADJUST 0x8A
#define VCNL4010_INTSTAT 0x8E
#define VCNL4010_MODTIMING 0x8F

typedef enum
  {
    VCNL4010_3M125   = 3,
    VCNL4010_1M5625  = 2,
    VCNL4010_781K25  = 1,
    VCNL4010_390K625 = 0,
  } vcnl4010_freq;

#define VCNL4010_MEASUREAMBIENT 0x10
#define VCNL4010_MEASUREPROXIMITY 0x08
#define VCNL4010_AMBIENTREADY 0x40
#define VCNL4010_PROXIMITYREADY 0x20

class VCNL4010 {
 public:
  VCNL4010();
  boolean begin(uint8_t a = VCNL4010_ADRESS);

  uint8_t getLEDcurrent(void);
  void setLEDcurrent(uint8_t c);

  void setFrequency(vcnl4010_freq f);
  uint16_t readProximity(void);
  uint16_t readAmbient(void);

 private:

  void write8(uint8_t address, uint8_t data);
  uint16_t read16(uint8_t address);
  uint8_t read8(uint8_t address);

  uint8_t _i2caddr;
};

#endif
