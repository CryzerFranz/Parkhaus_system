#ifndef MOTION_SENSOR_H
#define MOTION_SENSOR_H

class MotionSensor {
private:
  int pin;
  
public:
  MotionSensor(int pin);
  bool isMotion();

};

#endif // MQTT_CLIENT_H