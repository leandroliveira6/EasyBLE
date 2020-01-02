#include "ServiceBase.cpp"

#define STATE_ON "LIGADO"
#define STATE_OFF "DESLIGADO"

/**
 * @brief Modelo do LedService.
 *
 */
class LedService : public ServiceBase<String>
{
public:
  LedService(unsigned char pin);
  LedService(unsigned char pin, unsigned int period);
  LedService(unsigned char pin, std::string title, std::string subtitle);
  LedService(unsigned char pin, unsigned int period, std::string title, std::string subtitle);
  void init();
  void update();
  BLECharacteristic *getCharacteristicValue();

private:
  BLECharacteristic *_pCharacteristicValue;
};
