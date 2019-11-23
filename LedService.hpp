#include <BLEUtils.h>
#include <BLE2902.h>

#include "EasyBLE.hpp"

class LedService{  
  int _pin;
  
  public:
  LedService(int pin);
  void init();
  void update();
};
