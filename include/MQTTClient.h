#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <WiFi.h> // Use <WiFi.h> for ESP32
#include <PubSubClient.h>

class MQTTClient {
private:
  const char* ssid;
  const char* password;
  const char* mqttServer;
  int mqttPort;
  const char* clientId;
  
  WiFiClient wifiClient;
  PubSubClient mqttClient;
  unsigned long lastMsg;

  void setupWiFi();
  void callback(char* topic, byte* payload, unsigned int length);
  void reconnect();

public:
  MQTTClient(const char* wifiSsid, const char* wifiPassword, 
             const char* server, int port, const char* id);
  void begin();
  void run();
  void sendMessage(const char* topic,const char* msg);
};

#endif // MQTT_CLIENT_H