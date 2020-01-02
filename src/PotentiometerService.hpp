#include "ServiceBase.cpp"

/**
 * @brief Modelo de PotentiometerService.
 *
 */
class PotentiometerService : public ServiceBase<unsigned char>
{
  BLECharacteristic *_pCharacteristicValue;

public:
  PotentiometerService(unsigned char pin);
  PotentiometerService(unsigned char pin, unsigned int period);
  PotentiometerService(unsigned char pin, std::string title, std::string subtitle);
  PotentiometerService(unsigned char pin, unsigned int period, std::string title, std::string subtitle);
  void init();
  void update();
  BLECharacteristic *getCharacteristicValue();
};
