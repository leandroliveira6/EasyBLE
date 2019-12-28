#include <ESP32Servo.h>

#include "EasyBLE.hpp"

class ServoService
{
  int _pin;
  std::string _name;
  std::string _description;
  int _position;
  BLECharacteristic *_pCharacteristicOutput;
  Servo _servo;

public:
  ServoService(int pin, std::string name, std::string description);
  void init();
  void update();
  void changePosition(int newPosition);
  void publishPosition();
};
