#define BLYNK_TEMPLATE_ID "TMPLeKFBNU8k"
#define BLYNK_TEMPLATE_NAME "Room Control"
#define BLYNK_FIRMWARE_VERSION "1.1.1"
#include <IRremote.h>

//After connecting with the same wifi, search "192.168.1.70/update" to update code
//MAC Address "c8:c9:a3:06:55:e9"

IRrecv IR(D0);
decode_results results;

#define fan D1
#define light D5
#define pir D6                    //Motion sensor
#define led D7                    //Motion sensor indicator
#define charger D8

int firstBoot = 0;                //To check when it is booted
int remState = 0;                 //To check that remote is used or not after booting
unsigned long lastTime = 0;       //To check the holding of remote light button
unsigned long gap = 0;            //Gap to provide delay after holding the light button
bool refresh = 0;                 //Restart the MCU after reconnection with internet

bool fanState = 0;
bool chargerState = 0;
bool lightState = 0;
bool powerState = 0;
bool sensorState = 0;
bool winter = true;               //Change to true or false according to season

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"

BLYNK_WRITE(V1)                                         //Fan control
{
  fanState = param.asInt();
  digitalWrite(fan, fanState);

  if (firstBoot == 1)
  {
    if (fanState == 1 && powerState == 0)
    {
      Blynk.virtualWrite(V4, HIGH);
      powerState = 1;
    }
  
    else if (chargerState == 0 && lightState == 0)
    {
      Blynk.virtualWrite(V4, LOW);
      powerState = 0;
    } 
  }
}

BLYNK_WRITE(V2)                                         //Charger control
{
  chargerState = param.asInt();
  digitalWrite(charger, chargerState);
   
  if (firstBoot == 1)
  {
    if (chargerState == 1 && powerState == 0)
    {
      Blynk.virtualWrite(V4, HIGH);
      powerState = 1;
    }
  
    else if (fanState == 0 && lightState == 0)
    {
      Blynk.virtualWrite(V4, LOW);
      powerState = 0;
    }
  }
}

BLYNK_WRITE(V3)                                         //Light control
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

BLYNK_WRITE(V4)                                         //Power control
{
  powerState = fanState = chargerState = lightState = param.asInt();

  if (winter == false)
  {
    Blynk.virtualWrite(V1, fanState);
    digitalWrite(fan , fanState);
  }
  else if (powerState == 0)
  {
    Blynk.virtualWrite(V1, LOW);
    digitalWrite(fan, LOW);
  }

  Blynk.virtualWrite(V2, chargerState);
  Blynk.virtualWrite(V3, lightState);       

  digitalWrite(charger , powerState);
  if (sensorState == 0)
    digitalWrite(light, powerState);
}

BLYNK_WRITE(V5)
{
  sensorState = param.asInt();
}

BLYNK_CONNECTED()
{
  if (remState == 0)
  {
    Blynk.syncVirtual(V1, V2, V3, V5);
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
  
  Serial.begin(115200);
  delay(100);
  BlynkEdgent.begin();
  AsyncElegantOTA.begin(&server2);
  server2.begin();
}

void loop() 
{
  BlynkEdgent.run();
  ircontrol();
  motionSensor();
  
  if (refresh == 1 && Blynk.connected() && powerState == 0 && !digitalRead(pir))
  {
    restartMCU();
  }
}