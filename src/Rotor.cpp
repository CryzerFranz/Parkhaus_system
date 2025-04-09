#include "Rotor.h"

Rotor::Rotor(){
    this->s_rotor.attach(ROTOR_PIN);
}

void Rotor::open(){
    this->s_rotor.write(-180);
}

void Rotor::close(){
    this->s_rotor.write(80);
}