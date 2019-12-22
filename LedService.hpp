#include "EasyBLE.hpp"

class LedService{  
  int _pin;
  std::string _name;
  std::string _description;
  
  public:
  LedService(int pin, std::string name, std::string description);
  void init();
  void update();
};
