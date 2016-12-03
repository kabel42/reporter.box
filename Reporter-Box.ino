#include <math.h>
#include <algorithm>    // std::for_each
#include <vector>       // std::vector
#include <stdio.h>
#include <string>

#include "Sensor.h"

#include "SparkFunMicroOLED.h"

#include "HTU20D.h"
#include "ADC121C.h"
#include "tinyAudio.h"
#include "SoilSens.h"
#include "Relay.h"

std::vector<Sensor *> sensorList;

char data[255];
int delaytime = 60000;

#if Wiring_WiFi
STARTUP(WiFi.selectAntenna(ANT_EXTERNAL));
#endif

MicroOLED oled(MODE_I2C, D7, 0);

enum dsplStatus
{
  INIT,
  IN,
  OUT,
  CALIB,
} dspl;

void setupOled()
{
  oled.begin();    // Initialize the OLED
  //oled.setFontType(1);
}

int readIfInit(Sensor *S);

int readIfInit(Sensor *S)
{
  if(S)
  {
    if(S->initOK)
    {
      return S->read(data);
    }
  }
  return 0;
}

//Get Device Name
String devName = "Starting";
void handler(const char *topic, const char *data)
{
  devName = String(data);
}

bool pubTime = true;

void publishDataNow()
{
  for(Sensor *S: sensorList)
  {
    readIfInit(S);
  }
  pubTime = false;
}

void publishDataTimed()
{
  pubTime = true;
}

Timer publishTimer(delaytime, publishDataTimed);

//Audio
AudioSensor *audioSens;

void i2cTest()
{
  for(uint8_t i=1; i<128; i++)
  {
    Wire.beginTransmission(i);
    delay(10);
    int error = Wire.endTransmission();
    delay(10);

    if(error == 0)
    {
      snprintf(data, 250, "0x%X", i);
      Particle.publish("I2C", data);
      delay(1000);
    }
  }
}

//Relay
Relay *relay;

class Sensor * getSensorById(int id) {
  for(Sensor *S: sensorList)
  {
    if(S)
    {
      if(S->initOK)
      {
        if(S->_addr == id)
        {
          return S;
        }
      }
    }
  }
  return NULL;
}

#if Wiring_WiFi
int setWifi(String data)
{
  String ssid="", pw="";
  int i = 0;
  for(;(i<255) && (data[i] != '#'); i++)
  {
    if(data[i] == 0)
    {
      return -1;
    }
    ssid += data[i];
  }

  i++;

  for(;(i<255) && (data[i] != 0); i++)
  {
    pw += data[i];
  }
  WiFi.setCredentials(ssid, pw);
  return 0;
}

int getWifi(String data)
{
  WiFiAccessPoint ap[5];
  int found = WiFi.getCredentials(ap, 5);
  for (int i = 0; i < found; i++)
  {
    Particle.publish("SSID", ap[i].ssid);
  }
}
#endif

int calibrateSensor(String idIn)
{
  //Stop normal updates
  publishTimer.stop();

  long lId = idIn.toInt();
  char id[5];
  id[0] = (lId >> 24) & 0xFF;
  id[1] = (lId >> 16) & 0xFF;
  id[2] = (lId >>  8) & 0xFF;
  id[3] = (lId >>  0) & 0xFF;
  id[4] = 0;

  //get I2C Address
  int addr = id[0];
  Sensor *S = getSensorById(addr);
  int ret;

  if(S)
  {
    dspl = CALIB;
    oled.clear(PAGE);
    oled.setCursor(0, 0);  // Set the text cursor to the upper-left of the screen.
    oled.println("Calibrating...");
    char pubstring[15];
    snprintf(pubstring, 15, " 0x%X %s", id[0], &id[1]);
    oled.println(pubstring);
    oled.display();

    ret = S->getCal(id);
  }

  publishTimer.start();

  return ret;
}

int setSensorCal(String data)
{
  String tmp = "";
  int i = 0;
  float offset, scale;

  for(; i < 5 && data[i] != '#'; i++)
  {
    tmp += data[i];
  }

  long lId = tmp.toInt();
  char id[5];
  id[0] = (lId >> 24) & 0xFF;
  id[1] = (lId >> 16) & 0xFF;
  id[2] = (lId >>  8) & 0xFF;
  id[3] = (lId >>  0) & 0xFF;
  id[4] = 0;

  tmp = "";

  for(; i < data.length() && data[i] != '#'; i++)
  {
    tmp += data[i];
  }
  offset = tmp.toFloat();

  tmp = "";

  for(; i < data.length() && data[i] != 0; i++)
  {
    tmp += data[i];
  }
  scale = tmp.toFloat();

  Sensor *S = getSensorById(id[0]);
  return S->setCal(id, offset, scale);
}

void setup()
{
  Serial.begin();
  Serial.println("Get Name...");

  //Get Device Name
  Particle.subscribe("spark/", handler);
  Particle.publish("spark/device/name");
  //delay(1000);
  //Particle.unsubscribe();

  Serial.println("Init I2C");
  //Setup I2C
  Wire.setSpeed(10000);
  Wire.begin();

  Serial.println("init Audio");
  audioSens = new AudioSensor(0, delaytime);

  Serial.println("init OLED");
  setupOled();

  Serial.println("test I2C");
  //debug I2C
  i2cTest();

  //FIX AM2315
  Wire.beginTransmission(AM2315_I2CADDR);
  Wire.endTransmission();
  delay(500);
  //End FIX

  Serial.println("Init Sensors");
  //Init Sensors
  sensorList.push_back(new AM2315Sensor(0));
  sensorList.push_back(new HTU20DSensor(0));
  sensorList.push_back(new VCNL4010Sensor(0));
  sensorList.push_back(new ISL29125Sensor(0));
  sensorList.push_back(new MQ131Sensor(0x50));
  sensorList.push_back(new MQ4Sensor(0x51));
  sensorList.push_back(new MQ135Sensor(0x52));
  sensorList.push_back(new SoilSensor(0));

  publishTimer.start();

  pinMode(A3, INPUT_PULLDOWN); //RED Button
  pinMode(A4, INPUT_PULLDOWN); //Black Button

  //Relay
  relay = new Relay();

  Serial.println("register Cloud Functions");
  Particle.function("calibrate", calibrateSensor);
  Particle.function("setCal", setSensorCal);

#if Wiring_WiFi
  Particle.function("setWiFi", setWifi);
  Particle.function("getWiFi", getWifi);
#endif
  Serial.println("Setup Done");
}

void loop()
{
  //Check Timer
  if(pubTime)
    publishDataNow();

  //Audio
  if(audioSens->initOK)
    audioSens->poll();

  // Display stuff
  oled.clear(PAGE);
  oled.setCursor(0, 0);  // Set the text cursor to the upper-left of the screen.
  switch(dspl)
  {
    case INIT:
      if(&devName != NULL)
        oled.println(devName);
      oled.println("Running");
      #if Wiring_WiFi
      oled.println("on WiFi");
      #endif
      #if Wiring_Cellular
      oled.println("on Cellular");
      #endif

      if(digitalRead(A3))
      {
        dspl = IN;
      }
      if(digitalRead(A4))
      {
        dspl = OUT;
      }
      break;
    case CALIB:
      break;
    case IN:
      if(!digitalRead(A3))
      {
        dspl = INIT;
      }
      break;
    case OUT:
      oled.print("Out:");
      for(int i = 1; i<5; i++)
      {
        if(relay->getRelay(i))
        {
          oled.print("1");
        } else {
          oled.print("0");
        }
      }
      if(!digitalRead(A4))
      {
        dspl = INIT;
      }
      break;
    default:
      oled.println("???");
      dspl = INIT;
  };
  oled.display();
  delay(10);

  Serial.print(".");
}
