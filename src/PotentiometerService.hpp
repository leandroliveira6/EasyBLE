#include "ServiceBase.cpp"

/**
 * @brief Modelo do PotentiometerService.
 *
 */
class PotentiometerService : public ServiceBase<unsigned char>
{
public:
  PotentiometerService(unsigned char pin);
  PotentiometerService(unsigned char pin, unsigned int period);
  PotentiometerService(unsigned char pin, std::string title, std::string subtitle);
  PotentiometerService(unsigned char pin, unsigned int period, std::string title, std::string subtitle);
  void init();
  void update();

private:
  BLECharacteristic *_pCharacteristicValue;
};
