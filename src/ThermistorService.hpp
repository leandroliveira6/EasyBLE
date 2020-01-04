#include <esp_adc_cal.h>

#include "ServiceBase.cpp"

#define DEFAULT_BETA 3950
#define DEFAULT_MAX_VOLTAGE 5000
#define DEFAULT_MAX_SAMPLES 10

/**
 * @brief Modelo do ThermistorService.
 *
 */
class ThermistorService : public ServiceBase<float>
{
public:
  ThermistorService(unsigned char pin);
  ThermistorService(unsigned char pin, unsigned int period);
  ThermistorService(unsigned char pin, std::string title, std::string subtitle);
  ThermistorService(unsigned char pin, unsigned int period, std::string title, std::string subtitle);
  void init();
  void update();
  void setOptionals(unsigned short beta = DEFAULT_BETA, unsigned short maxVoltage = DEFAULT_MAX_VOLTAGE, unsigned short maxSamples = DEFAULT_MAX_SAMPLES);

private:
  BLECharacteristic *_pCharacteristicState;
  unsigned short _beta;
  unsigned short _maxVoltage;
  unsigned short _maxSamples;
  unsigned short _nSamples;
  unsigned int _sumSamples;
  esp_adc_cal_characteristics_t _calibration;
};
