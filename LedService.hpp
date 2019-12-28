#include "EasyBLE.hpp"

class LedService
{
  int _pin;
  std::string _name;
  std::string _description;
  bool _state;
  BLECharacteristic *_pCharacteristicOutput;

public:
  LedService(int pin, std::string name, std::string description);
  void init();
  void update();
  void changeState(bool newState);
  void publishState();
};
