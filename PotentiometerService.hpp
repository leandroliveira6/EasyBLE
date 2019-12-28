#include "EasyBLE.hpp"

class PotentiometerService
{
  int _pin;
  std::string _name;
  std::string _description;
  BLECharacteristic *_pCharacteristicOutput;

public:
  PotentiometerService(int pin, std::string name, std::string description);
  void init();
  void update();
};
