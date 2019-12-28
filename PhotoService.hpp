#include "EasyBLE.hpp"

class PhotoService
{
  int _pin;
  std::string _name;
  std::string _description;
  BLECharacteristic *_pCharacteristicOutput;

public:
  PhotoService(int pin, std::string name, std::string description);
  void init();
  void update();
};
