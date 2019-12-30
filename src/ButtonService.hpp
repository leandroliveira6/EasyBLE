#include "EasyBLE.hpp"

class ButtonService
{
  int _pin;
  std::string _name;
  std::string _description;
  BLECharacteristic *_pCharacteristicOutput;
  int _state; // button click count
  int _lastClickState;
  int _interval;
  int _lastMillis;

public:
  ButtonService(int pin, std::string name, std::string description);
  void init();
  void update();
  void changeState(int newState);
  void publishState();
  int getState();
};
