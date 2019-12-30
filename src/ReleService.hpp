#include "EasyBLE.hpp"

class ReleService
{
  int _pin;
  std::string _name;
  std::string _description;
  BLECharacteristic *_pCharacteristicOutput;
  bool _state; // rele state
  int _interval;
  int _lastMillis;

public:
  ReleService(int pin, std::string name, std::string description);
  void init();
  void update();
  void changeState(bool newState);
  void publishState();
};
