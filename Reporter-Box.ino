#include <math.h>
#include <algorithm>    // std::for_each
#include <vector>       // std::vector

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

MicroOLED oled(MODE_I2C, D7, 1);

enum dsplStatus
{
  INIT,
  IN,
  OUT,
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

void calibrateSensor(char *id)
{
  //Stop normal updates
  publishTimer.stop();

  //get I2C Address
  int addr = id[0];
  Sensor *S = getSensorById(addr);

  if(S)
  {
    //S->getCal(id); TODO
  }

  publishTimer.start();
}

void setup()
{
  //Get Device Name
  Particle.subscribe("spark/", handler);
  Particle.publish("spark/device/name");
  //delay(1000);
  //Particle.unsubscribe();

  //Setup I2C
  Wire.setSpeed(10000);
  Wire.begin();

  audioSens = new AudioSensor(0, delaytime);

  setupOled();

  //debug I2C
  i2cTest();

  //FIX AM2315
  Wire.beginTransmission(AM2315_I2CADDR);
  Wire.endTransmission();
  delay(500);
  //End FIX

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
      oled.println("Running...");
      break;
    default:
      oled.println("???");
      dspl = INIT;
  };
  oled.display();
  delay(10);
}
