#include "LCD.h"

LCDTimeDisplay::LCDTimeDisplay(uint8_t lcdAddr, uint8_t cols, uint8_t rows) 
    : lcdColumns(cols), lcdRows(rows), lastUpdate(0) {
    lcd = new LiquidCrystal_I2C(lcdAddr, lcdColumns, lcdRows);
    timeClient = new NTPClient(ntpUDP, "pool.ntp.org", 7200); // UTC+2 für Deutschland
}

void LCDTimeDisplay::begin() {
    lcd->init();
    lcd->backlight();
    timeClient->begin();
    
    // WLAN-Verbindung herstellen
    // WiFi.begin("DEIN_WLAN_NAME", "DEIN_WLAN_PASSWORT");
    
    // while (WiFi.status() != WL_CONNECTED) {
    //     delay(500);
    //     lcd->print("Verbinden...");
    // }
    
    lcd->clear();
    timeClient->update();
}

void LCDTimeDisplay::updateTime() {
    unsigned long currentMillis = millis();
    
    // Update nur alle 1000ms (1 Sekunde)
    if (currentMillis - lastUpdate >= 1000) {
        timeClient->update();
        
        lcd->setCursor(0, 0);
        lcd->print("Uhrzeit:");
        
        lcd->setCursor(0, 1);
        String timeStr = timeClient->getFormattedTime();
        lcd->print(timeStr);
        
        // Füllt restliche Zeile mit Leerzeichen
        for (int i = timeStr.length(); i < lcdColumns; i++) {
            lcd->print(" ");
        }
        
        lastUpdate = currentMillis;
    }
}

LCDTimeDisplay::~LCDTimeDisplay() {
    delete lcd;
    delete timeClient;
}