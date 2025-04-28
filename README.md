# Parkhaus System

Dieses Projekt implementiert ein Parkhaus-System für den ESP32, das verschiedene Sensoren, ein LCD-Display, Servos und LEDs verwendet, um den Ein- und Ausfahrtsprozess zu steuern. Es nutzt MQTT für die Kommunikation und zeigt Statusinformationen auf einem LCD an.

## Voraussetzungen

Vor dem Ausführen des Projekts muss eine `platformio.ini`-Datei erstellt werden. Diese Datei enthält die Konfiguration für die PlatformIO-Umgebung und die benötigten Bibliotheken.

Erstellen Sie die `platformio.ini` mit folgendem Inhalt:

```
[env:esp32dev]
platform = espressif32
board = esp32dev
monitor_speed = 115200
framework = arduino
lib_deps = 
    knolleary/PubSubClient@^2.8
    madhephaestus/ESP32Servo@^3.0.6
    marcoschwartz/LiquidCrystal_I2C@^1.1.4
    arduino-libraries/NTPClient@^3.2.1
build_flags = 
    '-D WLAN_SSID="DEIN_WLAN"'
    '-D WLAN_PASSWORD="DEIN_WLAN_PASSWORT"'
```

## Hardware-Pinbelegung

Die folgenden Module sind an spezifischen Pins des ESP32 angeschlossen:

| Modul              | Pin(s)         |
|--------------------|----------------|
| Bewegungssensor    |                |
| - Sensor rausfahren| G25            |
| - Sensor reinfahren| G26            |
| LCD Display        |                |
| - SDA              | G21            |
| - SCL              | G22            |
| Servo              | G16            |
| LED Rot            | G18            |
| LED Grün           | G19            |

## Installation und Ausführung

1. Klone das Repository: `git clone https://github.com/CryzerFranz/Parkhaus_system.git`
2. Erstelle die `platformio.ini`-Datei wie oben beschrieben.
3. Verbinde den ESP32 mit deinem Computer.
4. Lade das Projekt mit PlatformIO hoch: `pio run --target upload`
5. Überwache die serielle Ausgabe (falls nötig): `pio device monitor`

Stelle sicher, dass die WLAN-Zugangsdaten in den `build_flags` korrekt sind, um eine Verbindung zum Netzwerk herzustellen.

## Hinweise

- Die MQTT-Kommunikation basiert auf der PubSubClient-Bibliothek. Stelle sicher, dass ein MQTT-Broker verfügbar ist.
- Das LCD-Display verwendet die I2C-Schnittstelle zur Kommunikation.
- Die Servos und LEDs werden über die angegebenen GPIO-Pins angesteuert.
