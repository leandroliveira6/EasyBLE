#include "ServiceBase.cpp"

#define STATE_ON "LIGADO"
#define STATE_OFF "DESLIGADO"

class ReleService : public ServiceBase<String>
{
  BLECharacteristic *_pCharacteristicValue;

public:
  ReleService(unsigned char pin);
  ReleService(unsigned char pin, unsigned int period);
  ReleService(unsigned char pin, std::string title, std::string subtitle);
  ReleService(unsigned char pin, unsigned int period, std::string title, std::string subtitle);
  void init();
  void update();
  BLECharacteristic *getCharacteristicValue();
};
