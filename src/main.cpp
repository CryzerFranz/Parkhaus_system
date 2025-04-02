#include <Arduino.h>
// #include <WiFi.h>
#include "MQTTClient.h"

#define BWG_SENSOR_EINFAHRT 26


// Create an instance of MQTTClient
MQTTClient mqtt_client = MQTTClient("SCHB001", "schb001!", 
  "172.5.233.116", 1883, "ArduinoClient");

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mqtt_client.begin();
  pinMode(BWG_SENSOR_EINFAHRT, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  mqtt_client.run();
  int motion = digitalRead(BWG_SENSOR_EINFAHRT);  // Lese den Zustand des Sensors
  if (motion == HIGH) {
    mqtt_client.sendMessage("parkhaus/einfahrt","1");
    Serial.println("🚨 Bewegung erkannt!");
  } else {
    mqtt_client.sendMessage("parkhaus/einfahrt","0");
    Serial.println("🔵 Keine Bewegung");
  }
  delay(500);  // Kurze Pause, um das Log nicht zu überfluten
}

