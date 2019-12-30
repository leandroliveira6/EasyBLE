#include "EasyBLE.hpp"

class PhotoService
{
  int _pin;
  std::string _name;
  std::string _description;
  BLECharacteristic *_pCharacteristicOutput;
  int _state; // luminus rate
  int _interval;
  int _lastMillis;

public:
  PhotoService(int pin, std::string name, std::string description);
  void init();
  void update();
  void changeState(int newState);
  void publishState();
};
