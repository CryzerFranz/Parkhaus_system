#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include <Arduino.h>
#include "MQTTClient.h"

enum States_E { GRANTED, DENIED, CHECKING, IDLE, OPENING, CLOSING };
enum Events_E { IN_DETECT, OUT_DETECT, GRANTED_ACCESS, DENIED_ACCESS, RESET, OPEN, CLOSE };

class StateHandler {
private:
    States_E state;
    int car_count;
    StateHandler();

public:
    // Delete copy constructor and assignment operator
    StateHandler(const StateHandler&) = delete;
    StateHandler& operator=(const StateHandler&) = delete;

    static StateHandler& getInstance();

    void transition(Events_E event, MQTTClient* mqtt);
    States_E getState() const;
    void setState(States_E s);
    int getCarCount() const;
    void setCarCount(int value);
};

#endif // MQTT_CLIENT_H