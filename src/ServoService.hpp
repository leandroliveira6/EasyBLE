#include <ESP32Servo.h>

#include "ServiceBase.cpp"

#define DEFAULT_MIN_PULSE 500
#define DEFAULT_MAX_PULSE 2400

/**
 * @brief Modelo do ServoService.
 *
 */
class ServoService : public ServiceBase<unsigned char>
{
public:
  ServoService(unsigned char pin);
  ServoService(unsigned char pin, unsigned int period);
  ServoService(unsigned char pin, std::string title, std::string subtitle);
  ServoService(unsigned char pin, unsigned int period, std::string title, std::string subtitle);
  void init();
  void update();
  void setOptionals(unsigned short minPulseWidth = DEFAULT_MIN_PULSE, unsigned short maxPulseWidth = DEFAULT_MAX_PULSE);
  BLECharacteristic *getCharacteristicState();

private:
  BLECharacteristic *_pCharacteristicState;
  Servo _servo;
};
