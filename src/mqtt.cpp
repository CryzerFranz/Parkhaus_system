#include "MQTTClient.h"
#include "StateHandler.h"
#include <cstring>
#include <ArduinoJson.h>
#include "LCD.H"

MQTTClient::MQTTClient(const char* server, int port, const char* id) 
  : mqttServer(server), mqttPort(port), clientId(id), mqttClient(wifiClient), lastMsg(0), attempCount(0) {
}

void MQTTClient::callback(char* topic, byte* payload, unsigned int length) {
  States_E currentState = StateHandler::getInstance().getState();
  if (strcmp(topic, "parkhaus/einfahrt") == 0 && currentState == CHECKING) {
    char msg[256]; // Adjust size as needed
    if (length >= sizeof(msg)) length = sizeof(msg) - 1;
    memcpy(msg, payload, length);
    msg[length] = '\0';
    Serial.println(msg);

    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, msg);

    if (!error) {
      // Parsing successful, read "status" and "kennzeichen"
      const char* status = doc["status"];
      const char* plate = doc["kennzeichen"];
      LCDisplay::getInstance().updatePlate(plate);
      Serial.println("Attemp: ");
      Serial.println(attempCount);

      if (attempCount < 10) {
        if (strcmp(status, "denied") == 0) {
          attempCount++;
        } else {
          attempCount = 0;
          StateHandler::getInstance().transition(GRANTED_ACCESS, this);
        }
      } else {
        attempCount = 0;
        StateHandler::getInstance().transition(DENIED_ACCESS, this);
      }
    }
  }
}

void MQTTClient::reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect(clientId)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
  subscribe("parkhaus/einfahrt");
}

void MQTTClient::begin() {
  //setupWiFi();
  Serial.print("CONNECTED");
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback([this](char* topic, byte* payload, unsigned int length) {
    this->callback(topic, payload, length);
  });
  // while (!mqttClient.connected()) {
  //   Serial.print("🔄 Connecting to MQTT...");
  //   if (mqttClient.connect("ESP32Client")) { // Client ID
  //     Serial.println("✅ Connected to MQTT!");
  //   } else {
  //     Serial.print("❌ Failed, rc=");
  //     Serial.print(mqttClient.state());
  //     Serial.println(" Retrying...");
  //     delay(2000);
  //   }
  // }
}

void MQTTClient::run() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}

void MQTTClient::subscribe(const char* topic) {
  mqttClient.subscribe(topic);
}

void MQTTClient::sendMessage(const char* topic, const char* msg)
{
  bool success = mqttClient.publish(topic, msg);
  if(success)
  {
    Serial.println("Success");
  }
  else
  {
    Serial.println("Failed");
  }
}