#include "ServiceBase.cpp"

/**
 * @brief Modelo do ButtonService.
 *
 */
class ButtonService : public ServiceBase<int>
{
public:
  ButtonService(unsigned char pin);
  ButtonService(unsigned char pin, unsigned int period);
  ButtonService(unsigned char pin, std::string title, std::string subtitle);
  ButtonService(unsigned char pin, unsigned int period, std::string title, std::string subtitle);
  void init();
  void update();
  BLECharacteristic *getCharacteristicState();

private:
  BLECharacteristic *_pCharacteristicState;
};
