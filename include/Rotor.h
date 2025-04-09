#ifndef ROTOR_H
#define ROTOR_H

#include <ESP32Servo.h>
#define ROTOR_PIN 16

class Rotor {
private:
    Servo s_rotor;
public:
    Rotor();
    void open();
    void close();
};

#endif // ROTOR_H