#include "LCD.h"

LCDisplay::LCDisplay(uint8_t lcdAddr, uint8_t cols, uint8_t rows) 
    : lcdColumns(cols), lcdRows(rows), lastUpdate(0) {
    lcd = new LiquidCrystal_I2C(lcdAddr, lcdColumns, lcdRows);
    timeClient = new NTPClient(ntpUDP, "pool.ntp.org", 7200); // UTC+2 für Deutschland
}

void LCDisplay::begin() {
    lcd->init();
    lcd->backlight();
    timeClient->begin();
    
    lcd->clear();
    timeClient->update();
}

LCDisplay& LCDisplay::getInstance() {
    static LCDisplay instance;
    return instance;
}

void LCDisplay::updateTime() {
    unsigned long currentMillis = millis();
    
    // Update nur alle 1000ms (1 Sekunde)
    if (currentMillis - lastUpdate >= 1000) {
        timeClient->update();
        
        lcd->setCursor(0, 0);
        String timeStr = timeClient->getFormattedTime();
        lcd->print(timeStr);
        
        // lcd->setCursor(0, 1);
       
        
        // Füllt restliche Zeile mit Leerzeichen
        for (int i = timeStr.length(); i < lcdColumns; i++) {
            lcd->print(" ");
        }
        
        lastUpdate = currentMillis;
    }
}

void LCDisplay::updatePlate(const char* plate)
{
    lcd->setCursor(0, 1);
    lcd->print("                "); 
    lcd->setCursor(0, 1);
    lcd->print(plate);
}

LCDisplay::~LCDisplay() {
    delete lcd;
    delete timeClient;
}