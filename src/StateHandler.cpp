#include "StateHandler.h"

StateHandler::StateHandler() {
    state = IDLE;
    car_count = 0;
}

// Get the singleton instance
StateHandler& StateHandler::getInstance() {
    static StateHandler instance;
    return instance;
}

// Handle state transitions
void StateHandler::transition(Events_E event, MQTTClient* mqtt_client) {
    switch(event) {
        case IN_DETECT:
            state = CHECKING;
            mqtt_client->sendMessage("parkhaus/einfahrt_motion","1");
            break;
        case OUT_DETECT:
            state = GRANTED;
            break;
        case GRANTED_ACCESS:
            state = GRANTED;
            break;
        case DENIED_ACCESS:
            state = DENIED;
            break;
        case RESET:
            state = IDLE;
            break;
        default:
            break;
    }
}

// Get the current state
States_E StateHandler::getState() const {
    return state;
}

int StateHandler::getCarCount() const {
    return car_count;
}
// Manually set the state
void StateHandler::setState(States_E s) {
    state = s;
}

void StateHandler::setCarCount(int value) {
    car_count = value;
}
