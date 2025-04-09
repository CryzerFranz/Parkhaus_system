#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include <Arduino.h>
#include "MQTTClient.h"

enum States_E { GRANTED, DENIED, CHECKING, IDLE };
enum Events_E { IN_DETECT, OUT_DETECT, GRANTED_ACCESS, DENIED_ACCESS, RESET };

class StateHandler {
private:
    States_E state;

    // Private constructor for Singleton
    StateHandler();

public:
    // Delete copy constructor and assignment operator
    StateHandler(const StateHandler&) = delete;
    StateHandler& operator=(const StateHandler&) = delete;

    static StateHandler& getInstance();

    void transition(Events_E event, MQTTClient* mqtt);
    States_E getState() const;
    void setState(States_E s);
};

#endif // MQTT_CLIENT_H