#include "EasyBLE.hpp"

class ThermistorService
{
  int _pin;
  std::string _name;
  std::string _description;
  BLECharacteristic *_pCharacteristicOutput;
  float _state; // temperature value
  float _beta;
  int _interval;
  int _lastMillis;

public:
  ThermistorService(int pin, int beta, std::string name, std::string description);
  void init();
  void update();
  void changeState(float newState);
  void publishState();
};
