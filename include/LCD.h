#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

class LCDTimeDisplay {
private:
    LiquidCrystal_I2C* lcd;
    WiFiUDP ntpUDP;
    NTPClient* timeClient;
    uint8_t lcdColumns;
    uint8_t lcdRows;
    unsigned long lastUpdate;

public:
    LCDTimeDisplay(uint8_t lcdAddr = 0x27, uint8_t cols = 16, uint8_t rows = 2);
    void begin();
    void updateTime();
    ~LCDTimeDisplay();
};

#endif