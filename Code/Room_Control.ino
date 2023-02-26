#define BLYNK_TEMPLATE_ID "TMPLeKFBNU8k"
#define BLYNK_DEVICE_NAME "Room Control"
#include <IRremote.h>

IRrecv IR(D0);
decode_results results;

#define fan D1
#define charger D8
#define light D5
#define pir D6                    //Motion sensor
#define led D7                    //Motion sensor indicator

int firstBoot = 0;                //To check when it is booted
int remState = 0;                 //To check that remote is used or not after booting
unsigned long lastTime = 0;       //To check the double tap of remote light button

bool fanState = 0;
bool chargerState = 0;
bool lightState = 0;
bool powerState = 0;
bool sensorState = 0;

#define BLYNK_FIRMWARE_VERSION        "1.1.1"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  digitalWrite(fan , pinValue);
  fanState = pinValue;

  if (firstBoot == 1)
  {
    if (pinValue == 1)
    {
      Blynk.virtualWrite(V4, HIGH);
      powerState = 1;
    } 
  
    else if (fanState == 0 && chargerState == 0 && lightState == 0)
    {
      Blynk.virtualWrite(V4, LOW);
      powerState = 0;
    } 
  }
}

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt();
  digitalWrite(charger , pinValue);
  chargerState = pinValue;
   
  if (firstBoot == 1)
  {
    if (pinValue == 1)
    {
      Blynk.virtualWrite(V4, HIGH);
      powerState = 1;
    }
  
    else if (fanState == 0 && chargerState == 0 && lightState == 0)
    {
      Blynk.virtualWrite(V4, LOW);
      powerState = 0;
    }
  }
}

BLYNK_WRITE(V3)
{
  int pinValue = param.asInt();
  lightState = pinValue;

  if (lightState == 0 && sensorState == 1)
    digitalWrite(light, HIGH);
  else digitalWrite(light, lightState);
  
  if (firstBoot == 1)
  {
    if (pinValue == 1)
    {
      Blynk.virtualWrite(V4, HIGH);
      powerState = 1;
    }
  
    else if (fanState == 0 && chargerState == 0 && lightState == 0)
    {
      Blynk.virtualWrite(V4, LOW);
      powerState = 0;
    }
  }    
}

BLYNK_WRITE(V4)
{
  int pinValue = param.asInt();
    
//  Blynk.virtualWrite(V1, pinValue);                   //Comment this line for winters
  Blynk.virtualWrite(V2, pinValue);
  Blynk.virtualWrite(V3, pinValue);       
  
//  digitalWrite(fan , pinValue);                       //Comment this line for winters
  digitalWrite(charger , pinValue);
  
  if (pinValue == 0 && sensorState == 1)
    digitalWrite(light, HIGH);
  else digitalWrite(light, pinValue);
  
//  fanState = pinValue;                                //Comment this line for winters
  chargerState = pinValue;
  lightState = pinValue;
  powerState = pinValue;

  if (pinValue == 0)                                    //Uncomment this 'if condition' in winters
  {
    Blynk.virtualWrite(V1, 0);
    digitalWrite(fan, 0);
    fanState = 0;
  }
}

BLYNK_WRITE(V5)
{
  sensorState = param.asInt();
}

BLYNK_CONNECTED()
{
  if (remState == 0)
  {
    Blynk.syncVirtual(V1);
    Blynk.syncVirtual(V2);
    Blynk.syncVirtual(V3);
    Blynk.syncVirtual(V5);
  }

  else
  {
    Blynk.virtualWrite(V1, fanState);
    Blynk.virtualWrite(V2, chargerState);
    Blynk.virtualWrite(V3, lightState);
    Blynk.virtualWrite(V4, powerState);
    Blynk.virtualWrite(V5, sensorState);
  }
  firstBoot = 1;
}

void setup()
{
  pinMode(fan, OUTPUT);
  pinMode(charger, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(pir, INPUT);
  pinMode(led, OUTPUT);
  IR.enableIRIn();
  
  digitalWrite(fan, LOW);
  digitalWrite(charger, LOW);
  digitalWrite(light, LOW);
  digitalWrite(led, LOW);
  
  Serial.begin(9600);
  delay(100);
  BlynkEdgent.begin();
}

void loop() 
{
  BlynkEdgent.run();
  ircontrol();
  motionSensor();
}
