#include "Sensor.h"

void publishData(uint8_t addr, char* measID, float raw, float offset, float scale, char* sens)
{
  static char pubstring[252];
  //snprintf(pubstring, 250, "{\"uncalibrated_value\": %f, \"calibrated_value\": %f, \"Measurement ID\": \"%s\", \"sensor\": {\"address:\"%d, \"name\":%s}}, ", raw, data, measID, addr, sens);

  //Gen Id from addr and measID
  uint32_t id = addr<<(3*8);
  int len = strlen(measID);
  if(len>3)
    len = 3;

  for(int i = 0; i<len; i++)
  {
    id += measID[i]<<(8*(2-i));
  }

  char rawStr[35];
  char dataStr[35];

  if(isfinite(raw))
  {
    snprintf(rawStr, 34, "\"uncalibrated_value\": %.2f,", raw);

    float data = 0;
    if(scale != 0)
    {
      data = (raw+offset)*scale+1;
      if(data<1)
      {
        data = 1;
      } else if(data>10)
      {
        data = 10;
      }
    }

    snprintf(dataStr, 34, "\"calibrated_value\": %.2f,", data);
  } else {
    snprintf(rawStr, 34, "");
    snprintf(dataStr, 34, "");
  }

  snprintf(pubstring, 250, "{%s %s \"sensor\": {\"address\": %li}}", rawStr, dataStr, id);

  Particle.publish("measurement", pubstring);
  Serial.print(addr);
  Serial.print(", ");
  Serial.print(measID);
  Serial.print(", ");
  Serial.print(raw);
  Serial.print(", ");
  Serial.println(pubstring);
  delay(1000);
}

void publishCal(char* measID, float min, float max, float offset, float scale)
{
  char pubstring[100];
  //Gen Id from addr and measID
  uint32_t id = 0;
  int len = strlen(measID);
  if(len>4)
    len = 4;

  for(int i = 0; i<len; i++)
  {
    id += measID[i]<<(8*(3-i));
  }

  snprintf(pubstring, 100, "{\"min\": %.2f, \"max\": %.2f, \"sensor\": {\"address\": %li, \"offset\": %.2f, \"scale\": %.2f}}", min, max, id, offset, scale);

  Particle.publish("calibration", pubstring);
}

bool calLoop(Sensor *S, char *id, float *offset, float *scale)
{
  bool ret = false;
  float min, max;

  float cur = S->getVal(id);
  min = max = cur;

  long end = millis() + CALTIME;
  while((long)(end - millis()) > 0)
  {
    cur = S->getVal(id);
    if(isfinite(cur))
    {
      ret = true;
      if(cur < min) {
        min = cur;
      } else if(cur > max) {
        max = cur;
      }
    }
    delay(1);
  }

  *offset = (-min);
  *scale = 9/(max-min);

  publishCal(id, min, max, *offset, *scale);

  return ret;
}

float Sensor::getVal(char *id)
{
  return read();
}

NULLSensor::NULLSensor(int addr)
{
  _addr = addr;
}

int NULLSensor::read(char* status)
{
  return true;
}

float NULLSensor::read()
{
  return nanf("NA");
}

bool NULLSensor::getCal(char *id)
{
  return false;
}

bool NULLSensor::setCal(char* id, float offset, float scale)
{
  return false;
}

VCNL4010Sensor::VCNL4010Sensor(int addr): drv()
{
  if(addr != 0)
  {
    _addr = addr;
  } else {
    _addr = VCNL4010_ADRESS;
  }

  //Check addr
  Wire.beginTransmission(_addr);
  int error = Wire.endTransmission();

  if(error == 0)
  {
    initOK = (drv.begin(_addr));
  }
}

float VCNL4010Sensor::read()
{
  if(initOK)
  {
    //return drv.readAmbient();
    return drv.readAmbient();
  }
  return nanf("NA");
}

int VCNL4010Sensor::read(char* status)
{
  if(initOK)
  {
    publishData(VCNL4010_ADRESS, "AMB", drv.readAmbient(), offsetAMB, scaleAMB, "VCNL4010");
    publishData(VCNL4010_ADRESS, "PRX", drv.readProximity(), offsetPRX, scalePRX, "VCNL4010");
    return 0;
  }
  return -1;
}

bool VCNL4010Sensor::getCal(char *id)
{
  if(id[1] == 'A' && id[2] == 'M' && id[3] == 'B') {
    return calLoop(this, id, &offsetAMB, &scaleAMB);
  } else if(id[1] == 'P' && id[2] == 'R' && id[3] == 'X') {
    return calLoop(this, id, &offsetPRX, &scalePRX);
  } else {
    return false;
  }
}
bool VCNL4010Sensor::setCal(char* id, float offset, float scale)
{
  if(id[1] == 'A' && id[2] == 'M' && id[3] == 'B') {
    offsetAMB = offset;
    scaleAMB  = scale;
    return true;
  } else if(id[1] == 'P' && id[2] == 'R' && id[3] == 'X') {
    offsetPRX = offset;
    scalePRX = scale;
    return true;
  } else {
    return false;
  }
}


float VCNL4010Sensor::getVal(char *id)
{
  if(id[1] == 'A' && id[2] == 'M' && id[3] == 'B') {
    return drv.readAmbient();
  } else if(id[1] == 'P' && id[2] == 'R' && id[3] == 'X') {
    return drv.readProximity();
  } else {
    return nanf("NA");
  }
}

AnalogSensor::AnalogSensor(int addr)
{
  _addr   = addr;
  initOK = true;
}

float AnalogSensor::read()
{
  if(initOK)
  {
    //FIXME: return analogRead(_addr);
  }
  return nanf("NA");
}

int AnalogSensor::read(char* status)
{
  if(initOK)
  {
    //FIXME: sprintf(status, "ANA#%i#?", analogRead(_addr));
    return 0;
  }
  return -1;
}

AM2315Sensor::AM2315Sensor(int addr): drv()
{
  if(addr != 0)
  {
    _addr = addr;
  } else {
    _addr = AM2315_I2CADDR;
  }

  //Check addr
  Wire.beginTransmission(_addr);
  int error = Wire.endTransmission();

  if(error == 0)
  {
    initOK = (drv.begin());
  }
}

float AM2315Sensor::read()
{
  if(initOK)
  {
    //return drv.readAmbient();
    return drv.readTemperature();
  }
  return nanf("NA");
}

int AM2315Sensor::read(char* status)
{
  if(initOK)
  {
    //sprintf(status, "AMB#%i#?##DIST#%i#?", drv.readAmbient(), drv.readProximity());
    float tmp, rh;
    if(drv.readTemperatureAndHumidity(&tmp, &rh)) {
      publishData(_addr, "TMP", tmp, offsetTMP, scaleTMP, "AM2315");
      publishData(_addr, "RH", rh, offsetRH, scaleRH, "AM2315");
      return 0;
    } else {
      return -1;
    }
  //  publishData(_addr, "TMP", drv.readTemperature(), offsetTMP, scaleTMP, "AM2315");
  //  publishData(_addr, "RH", drv.readHumidity(), offsetRH, scaleRH, "AM2315");
    return 0;
  }
  return -1;
}

float AM2315Sensor::getVal(char *id)
{
  if(id[1] == 'R' && id[2] == 'H') {
    return drv.readHumidity();
  } else if(id[1] == 'T' && id[2] == 'M' && id[3] == 'P') {
    return drv.readTemperature();
  } else {
    return nanf("NA");
  }
}

bool AM2315Sensor::getCal(char *id)
{
  if(id[1] == 'R' && id[2] == 'H') {
    return calLoop(this, id, &offsetRH, &scaleRH);
  } else if(id[1] == 'T' && id[2] == 'M' && id[3] == 'P') {
    return calLoop(this, id, &offsetTMP, &scaleTMP);
  } else {
    return false;
  }
}

bool AM2315Sensor::setCal(char* id, float offset, float scale)
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


ISL29125Sensor::ISL29125Sensor(int addr): drv()
{
  if(addr != 0)
  {
    _addr = addr;
  } else {
    _addr = ISL_I2C_ADDR;
  }

  //Check addr
  Wire.beginTransmission(_addr);
  int error = Wire.endTransmission();

  if(error == 0)
  {
    initOK = (drv.init());
  }
}

float ISL29125Sensor::read()
{
  if(initOK)
  {
    return drv.readRed()+drv.readBlue()+drv.readGreen();
  }
  return nanf("NA");
}

int ISL29125Sensor::read(char* status)
{
  if(initOK)
  {
    float red   = drv.readRed();
    float green = drv.readGreen();
    float blue  = drv.readBlue();
    float mul   = 10./65535.;
    publishData(_addr, "R", red, offsetR, scaleR, "ISE29125");
    publishData(_addr, "G", green, offsetG, scaleG, "ISE29125");
    publishData(_addr, "B", blue, offsetB, scaleB, "ISE29125");
    return 0;
  }
  return -1;
}

float ISL29125Sensor::getVal(char *id)
{
  if(id[1] == 'R') {
    return drv.readRed();
  } else if(id[1] == 'G') {
    return drv.readGreen();
  } else if(id[1] == 'B') {
    return drv.readGreen();
  } else {
    return nanf("NA");
  }
}

bool ISL29125Sensor::getCal(char *id)
{
  if(id[1] == 'R') {
    return calLoop(this, id, &offsetR, &scaleR);
  } else if(id[1] == 'G') {
    return calLoop(this, id, &offsetG, &scaleG);
  } else if(id[1] == 'B') {
    return calLoop(this, id, &offsetB, &scaleB);
  } else {
    return false;
  }
}

bool ISL29125Sensor::setCal(char* id, float offset, float scale)
{
  if(id[1] == 'R') {
    offsetR = offset;
    scaleR  = scale;
    return true;
  } else if(id[1] == 'G') {
    offsetG = offset;
    scaleG  = scale;
    return true;
  } else if(id[1] == 'B') {
    offsetB = offset;
    scaleB  = scale;
    return true;
  } else {
    return false;
  }
}
