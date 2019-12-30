#include "EasyBLE.hpp"

class LedService
{
  int _pin;
  std::string _name;
  std::string _description;
  BLECharacteristic *_pCharacteristicOutput;
  bool _state; // led state
  int _interval;
  int _lastMillis;

public:
  LedService(int pin, std::string name, std::string description);
  void init();
  void update();
  void changeState(bool newState);
  void publishState();
};
