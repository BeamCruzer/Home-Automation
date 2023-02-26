
extern "C" {
  #include "user_interface.h"
  void app_loop();
}

#include "Settings.h"
#include <BlynkSimpleEsp8266_SSL.h>

#ifndef BLYNK_NEW_LIBRARY
#error "Old version of Blynk library is in use. Please replace it with the new one."
#endif

#if !defined(BLYNK_TEMPLATE_ID) || !defined(BLYNK_DEVICE_NAME)
#error "Please specify your BLYNK_TEMPLATE_ID and BLYNK_DEVICE_NAME"
#endif

#include "BlynkState.h"
#include "ConfigStore.h"
#include "ResetButton.h"
#include "ConfigMode.h"
#include "Indicator.h"
#include "OTA.h"
#include "Console.h"

void ircontrol();
void motionSensor();
inline
void BlynkState::set(State m) {
  if (state != m && m < MODE_MAX_VALUE) {
    DEBUG_PRINT(String(StateStr[state]) + " => " + StateStr[m]);
    state = m;

    // You can put your state handling here,
    // i.e. implement custom indication
  }
}

void printDeviceBanner()
{
  Blynk.printBanner();
  DEBUG_PRINT("--------------------------");
  DEBUG_PRINT(String("Product:  ") + BLYNK_DEVICE_NAME);
  DEBUG_PRINT(String("Firmware: ") + BLYNK_FIRMWARE_VERSION " (build " __DATE__ " " __TIME__ ")");
  if (configStore.getFlag(CONFIG_FLAG_VALID)) {
    DEBUG_PRINT(String("Token:    ...") + (configStore.cloudToken+28));
  }
  DEBUG_PRINT(String("Device:   ") + BLYNK_INFO_DEVICE + " @ " + ESP.getCpuFreqMHz() + "MHz");
  DEBUG_PRINT(String("MAC:      ") + WiFi.macAddress());
  DEBUG_PRINT(String("Flash:    ") + ESP.getFlashChipRealSize() / 1024 + "K");
  String coreVer = ESP.getCoreVersion();
  coreVer.replace("_", ".");
  DEBUG_PRINT(String("ESP core: ") + coreVer);
  DEBUG_PRINT(String("ESP SDK:  ") + ESP.getSdkVersion());
  DEBUG_PRINT(String("Boot Ver: ") + ESP.getBootVersion());
  DEBUG_PRINT(String("Boot Mode:") + ESP.getBootMode());
  DEBUG_PRINT(String("FW info:  ") + ESP.getSketchSize() + "/" + ESP.getFreeSketchSpace() + ", MD5:" + ESP.getSketchMD5());
  DEBUG_PRINT(String("Free mem: ") + ESP.getFreeHeap());
  DEBUG_PRINT("--------------------------");
}

void runBlynkWithChecks() {
  Blynk.run();
  if (BlynkState::get() == MODE_RUNNING) {
    if (!Blynk.connected()) {
      if (WiFi.status() == WL_CONNECTED) 
        BlynkState::set(MODE_CONNECTING_CLOUD);
      else BlynkState::set(MODE_CONNECTING_NET);
    }
  }
}

class Edgent {

public:
  void begin()
  {
    indicator_init();
    button_init();
    config_init();
    console_init();

    printDeviceBanner();

    if (configStore.getFlag(CONFIG_FLAG_VALID)) {
      BlynkState::set(MODE_CONNECTING_NET);
    } else if (config_load_blnkopt()) {
      DEBUG_PRINT("Firmware is preprovisioned");
      BlynkState::set(MODE_CONNECTING_NET);
    } else {
      BlynkState::set(MODE_WAIT_CONFIG);
    }
  }

  void run() {
    app_loop();
    switch (BlynkState::get()) {
    case MODE_WAIT_CONFIG:       
    case MODE_CONFIGURING:       enterConfigMode();    break;
    case MODE_CONNECTING_NET:    enterConnectNet();    break;
    case MODE_CONNECTING_CLOUD:  enterConnectCloud();  break;
    case MODE_RUNNING:           runBlynkWithChecks(); break;
    case MODE_OTA_UPGRADE:       enterOTA();           break;
    case MODE_SWITCH_TO_STA:     enterSwitchToSTA();   break;
    case MODE_RESET_CONFIG:      enterResetConfig();   break;
    default:                     enterError();         break;
    }
  }

};

Edgent BlynkEdgent;
BlynkTimer edgentTimer;

void app_loop() 
{
  edgentTimer.run();
  edgentConsole.run();
  ircontrol();
  motionSensor();
}

void ircontrol()
{ 
  if(IR.decode())
  {
    Serial.println(IR.decodedIRData.decodedRawData, HEX);
    if(IR.decodedIRData.decodedRawData == 0xF8070707)                   //Fan Control
    {
      remState = 1;
      fanState = !fanState;
      Blynk.virtualWrite(V1, fanState);
      digitalWrite(fan, fanState);

      if (fanState == 1)
      {
        Blynk.virtualWrite(V4, HIGH);
        powerState = 1;
      } 
    
      else if (fanState == 0 && chargerState == 0 && lightState == 0)
      {
        Blynk.virtualWrite(V4, LOW);
        powerState = 0;
      } 
      delay(100);
    }

    if(IR.decodedIRData.decodedRawData == 0xF00F0707)                   //Charger control
    {
      remState = 1;
      chargerState = !chargerState;
      Blynk.virtualWrite(V2, chargerState);
      digitalWrite(charger, chargerState);

      if (chargerState == 1)
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

    if(IR.decodedIRData.decodedRawData == 0xF40B0707)                   //Light control
    {
      remState = 1;
      lightState = !lightState;
      Blynk.virtualWrite(V3, lightState);

      if (millis() - lastTime <= 600)                                   //Motion sensor control
      {
        sensorState = !sensorState;
        Blynk.virtualWrite(V5, sensorState);
      }

      if (lightState == 0 && sensorState == 1)
        digitalWrite(light, HIGH);
      else digitalWrite(light, lightState);
      
      if (lightState == 1)
      {
        Blynk.virtualWrite(V4, HIGH);
        powerState = 1;
      } 
    
      else if (fanState == 0 && chargerState == 0 && lightState == 0)
      {
        Blynk.virtualWrite(V4, LOW);
        powerState = 0;
      }

      lastTime = millis();
      delay(50);
    }

    if(IR.decodedIRData.decodedRawData == 0xFD020707)                   //Power control
    {
      remState = 1;
      powerState = !powerState;
      lightState = powerState;
//      Blynk.virtualWrite(V1, powerState);                               //Comment this line for winters
      Blynk.virtualWrite(V2, powerState);
      Blynk.virtualWrite(V3, powerState);
      Blynk.virtualWrite(V4, powerState);
      
//      digitalWrite(fan , powerState);                                   //Comment this line for winters
      digitalWrite(charger , powerState);

      if (powerState == 0 && sensorState == 1)
        digitalWrite(light, HIGH);
      else digitalWrite(light, powerState);

      if (powerState == 0)                                                //Uncomment this if condition in winters
      {
        Blynk.virtualWrite(V1, 0);
        digitalWrite(fan , 0);
        fanState = 0;
      }
    } 
    IR.resume();
  }
}

void motionSensor()
{
  if (firstBoot == 1)
  {
    if (sensorState == 1 && lightState == 0)
    {
      if (digitalRead(pir) == HIGH)
      {
        digitalWrite(light, HIGH);
        digitalWrite(led, HIGH);
      }
              
      else 
      {
        digitalWrite(light, LOW);
        digitalWrite(led, LOW);
      }
    }

    else if (sensorState == 0 || lightState == 1)
    {
      digitalWrite(light, lightState);
      digitalWrite(led, LOW);
    }
  } 
}
