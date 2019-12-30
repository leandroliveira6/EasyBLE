#include <ESP32Servo.h>

#include "EasyBLE.hpp"

class ServoService
{
  int _pin; //All pin numbers are allowed, but only pins 2,4,12-19,21-23,25-27,32-33 are recommended.
  std::string _name;
  std::string _description;
  BLECharacteristic *_pCharacteristicOutput;
  int _state; // servo position
  Servo _servo;
  int _interval;
  int _lastMillis;

public:
  ServoService(int pin, std::string name, std::string description);
  void init();
  void update();
  void changeState(int newState);
  void publishState();
};
