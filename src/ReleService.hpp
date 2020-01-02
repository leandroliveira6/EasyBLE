#include "ServiceBase.cpp"

#define STATE_ON "LIGADO"
#define STATE_OFF "DESLIGADO"

/**
 * @brief Modelo de ReleService.
 *
 */
class ReleService : public ServiceBase<String>
{
public:
  ReleService(unsigned char pin);
  ReleService(unsigned char pin, unsigned int period);
  ReleService(unsigned char pin, std::string title, std::string subtitle);
  ReleService(unsigned char pin, unsigned int period, std::string title, std::string subtitle);
  void init();
  void update();
  BLECharacteristic *getCharacteristicValue();

private:
  BLECharacteristic *_pCharacteristicValue;
};
