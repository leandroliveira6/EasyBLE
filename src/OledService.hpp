#include <SSD1306.h>

#include "ServiceBase.cpp"

#define SDA_PIN 5
#define SCL_PIN 4

/**
 * @brief Modelo do OledService.
 *
 */
class OledService : public ServiceBase<String>
{
public:
  OledService();
  OledService(unsigned int period);
  OledService(std::string title, std::string subtitle);
  OledService(unsigned int period, std::string title, std::string subtitle);
  void init();
  void update();
  void setOptionals(unsigned char sda = SDA_PIN, unsigned char scl = SCL_PIN);
  SSD1306 *getDisplay();
  void setDisplay(String msg);

private:
  BLECharacteristic *_pCharacteristicState;
  unsigned char _sda, _scl;
  SSD1306 *_display;
};
