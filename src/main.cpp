#include <Arduino.h>
#include "MQTTClient.h"
#include "MotionSensor.h"
#include "StateHandler.h"
#include "Rotor.h"

#define BWG_SENSOR_EINFAHRT 26
#define BWG_SENSOR_AUSFAHRT 25

MotionSensor in_motion = MotionSensor(BWG_SENSOR_EINFAHRT);
MotionSensor out_motion = MotionSensor(BWG_SENSOR_AUSFAHRT);

// Create an instance of MQTTClient
MQTTClient mqtt_client = MQTTClient("SCHB001", "schb001!", 
  "172.5.232.218", 1883, "ArduinoClient");

StateHandler& stateHandler = StateHandler::getInstance();
Rotor schranke = Rotor();

void setup() {
  Serial.begin(115200);
  mqtt_client.begin();
}

void loop() {
  mqtt_client.run();
  switch(stateHandler.getState())
  {
    case IDLE:
      if(in_motion.isMotion())
      {
        stateHandler.transition(IN_DETECT, &mqtt_client);
        Serial.println("HALLO");
      }
      if(out_motion.isMotion())
      {
        stateHandler.transition(OUT_DETECT, &mqtt_client);
        Serial.println("Tschuesss");
      }
      delay(500);
      break;
    case CHECKING:
        
      break; 
    case GRANTED:
      Serial.println("ACCESSSSSSSS");
      schranke.open();
      delay(5000);
      schranke.close();
      stateHandler.transition(RESET, &mqtt_client);
      break;
  }
}

