#include "MotionSensor.h"
#include <Arduino.h>

MotionSensor::MotionSensor(int pin)
{
    this->pin = pin;
    pinMode(pin, INPUT);
}

bool MotionSensor::isMotion(){
    return digitalRead(this->pin);
}