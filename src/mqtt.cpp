#include "MQTTClient.h"
#include "StateHandler.h"
#include <cstring>

MQTTClient::MQTTClient(const char* server, int port, const char* id) 
  : mqttServer(server), mqttPort(port), clientId(id), mqttClient(wifiClient), lastMsg(0), attempCount(0) {
}

// void MQTTClient::setupWiFi() {
//   delay(10);

//   Serial.println("Entering setupWiFi");
//   delay(10);
//   Serial.print("SSID is: ");
//   Serial.println(ssid ? ssid : "NULL"); // Check if ssid is valid
//   Serial.print("Password is: ");
//   Serial.println(password ? password : "NULL"); // Check if password is valid
//   Serial.println("Calling WiFi.begin");
//   //WiFi.begin(const_cast<char*>(ssid), const_cast<char*>(password));
//   WiFi.begin("SCHB001", "schb001!");
//   Serial.println("WiFi.begin executed");

//   Serial.print("Connecting to ");
//   Serial.println(ssid);

//   WiFi.begin(const_cast<char*>(ssid), password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("");
//   Serial.println("Wi-Fi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }

void MQTTClient::callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("TOPIC: ");
  Serial.println(topic);
  States_E currentState = StateHandler::getInstance().getState();
  if (strcmp(topic, "parkhaus/einfahrt") == 0 && currentState == CHECKING) {
    char msg[32]; // Statischer Buffer, Größe anpassen (z. B. max. Payload-Länge)
    if (length >= sizeof(msg)) length = sizeof(msg) - 1; // Begrenze Länge
    memcpy(msg, payload, length);
    msg[length] = '\0';
    Serial.println(msg);

    if (attempCount < 10) {
      if (strcmp(msg, "denied") == 0) {
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