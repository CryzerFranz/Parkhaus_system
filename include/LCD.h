#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

class LCDisplay {
private:
    LiquidCrystal_I2C* lcd;
    WiFiUDP ntpUDP;
    NTPClient* timeClient;
    uint8_t lcdColumns;
    uint8_t lcdRows;
    unsigned long lastUpdate;
    LCDisplay(uint8_t lcdAddr = 0x27, uint8_t cols = 16, uint8_t rows = 2);

public:
    LCDisplay(const LCDisplay&) = delete;
    LCDisplay& operator=(const LCDisplay&) = delete;
    static LCDisplay& getInstance();

    void begin();
    void updateTime();
    void updatePlate(const char* plate);
    ~LCDisplay();
};

#endif