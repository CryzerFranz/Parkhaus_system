#include <Arduino.h>
#include "MQTTClient.h"
#include "MotionSensor.h"
#include "StateHandler.h"
#include "Rotor.h"
#include <WiFi.h>
#include "LCD.h"

#define BWG_SENSOR_EINFAHRT 26
#define BWG_SENSOR_AUSFAHRT 25
#define MANUELLE_STEUERUNG_SCHRANKE 12

#define LED_ROT 18
#define LED_GRUEN 19

MotionSensor in_motion = MotionSensor(BWG_SENSOR_EINFAHRT);
MotionSensor out_motion = MotionSensor(BWG_SENSOR_AUSFAHRT);

// Create an instance of MQTTClient
MQTTClient mqtt_client = MQTTClient(
  "172.5.232.218", 1883, "ArduinoClient");

StateHandler& stateHandler = StateHandler::getInstance();
Rotor schranke = Rotor();

LCDisplay& display = LCDisplay::getInstance();

long prevTime = -1;

void setup() {
  Serial.begin(115200);

  //WLAN

  delay(10);

  Serial.println("Entering setupWiFi");
  delay(10);
  WiFi.begin(WLAN_SSID, WLAN_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  delay(1000);
  mqtt_client.begin();
  display.begin();

  pinMode(MANUELLE_STEUERUNG_SCHRANKE, INPUT);
  pinMode(LED_ROT, OUTPUT);
  pinMode(LED_GRUEN, OUTPUT);
}

bool checkTimePass()
{
  long diffTime = millis() - prevTime;
  if(diffTime > 4000)
  {
    return true;
  }
  return false;
}

void switchStateAfterTimePassed(Events_E event)
{
  stateHandler.transition(event, &mqtt_client);
  prevTime = -1;
}

void loop() {
  while(WiFi.status() != WL_CONNECTED) {
    Serial.println("🔌 WiFi disconnected! Trying to reconnect...");
    WiFi.disconnect();
    WiFi.reconnect();
    delay(3000);
  }
  display.updateTime();
  int carCount = stateHandler.getCarCount();

  mqtt_client.run();
  if(carCount >= 2)
  {
    digitalWrite(LED_ROT, HIGH);
    digitalWrite(LED_GRUEN, LOW);
  }
  else
  {
    digitalWrite(LED_ROT, LOW);
    digitalWrite(LED_GRUEN, HIGH);
  }

  // if (digitalRead(MANUELLE_STEUERUNG_SCHRANKE) == LOW) {
  //   Serial.println("hallo");
  //     stateHandler.transition(OPEN, &mqtt_client);
  // }
  // unsigned long currentTime = millis();
  switch(stateHandler.getState())
  {
    case IDLE:
      if(carCount < 2)
      {
        if(in_motion.isMotion())
        {
          stateHandler.transition(IN_DETECT, &mqtt_client);
        }
      }
      if(out_motion.isMotion())
      {
        if(carCount > 0)
        {
          stateHandler.setCarCount(carCount - 1);
        }
        stateHandler.transition(OPEN, &mqtt_client);
        // schranke.open();
        // delay(5000);
        // schranke.close();
      }
      delay(500);
      break;
    case CHECKING:
        Serial.print(".");
      break; 
    case GRANTED:
      Serial.println("ACCESS GRANTED");
      stateHandler.setCarCount(carCount + 1);
      // schranke.open();
      // delay(5000);
      // schranke.close();
      stateHandler.transition(OPEN, &mqtt_client);
      break;
    case OPENING:
      Serial.println("OPENING THE GATE");
      if(prevTime == -1)
      {
        schranke.open();
        prevTime = millis();
      }
      if(checkTimePass())
      {
        switchStateAfterTimePassed(CLOSE);
      }
      break;
    case CLOSING:
      display.updatePlate("");
      if(prevTime == -1)
      {
        prevTime = millis();
        schranke.close();
      }
      if(checkTimePass())
      {
        switchStateAfterTimePassed(RESET);
      }
      break;
    case DENIED:
      stateHandler.transition(RESET, &mqtt_client);

  }
}

