#include <math.h>
#include <algorithm>    // std::for_each
#include <vector>       // std::vector

#include "SparkFunMicroOLED.h"

#include "Sensor.h"
#include "HTU20D.h"
#include "ADC121C.h"
#include "tinyAudio.h"
#include "SoilSens.h"

std::vector<Sensor*> sensorList;

char data[255];
int delaytime = 60000;

MicroOLED oled(MODE_I2C, D7, 0);

enum dsplStatus
{
  INIT,
  IN,
  OUT,
} dspl;

void setupOled()
{
  oled.begin();    // Initialize the OLED
  oled.setFontType(1);
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
String devName = "???";
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
AudioSensor audio(0, delaytime);

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

  setupOled();

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

}

void loop()
{
  //Check Timer
  if(pubTime)
    publishDataNow();

  //Audio
  audio.poll();

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
