#include "ThermistorService.hpp"

ThermistorService::ThermistorService(unsigned char pin) : ThermistorService::ThermistorService(pin, DEFAULT_PERIOD, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ThermistorService::ThermistorService(unsigned char pin, unsigned int period) : ThermistorService::ThermistorService(pin, period, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ThermistorService::ThermistorService(unsigned char pin, std::string title, std::string subtitle) : ThermistorService::ThermistorService(pin, DEFAULT_PERIOD, title, subtitle) {}
ThermistorService::ThermistorService(unsigned char pin, unsigned int period, std::string title, std::string subtitle) : ServiceBase::ServiceBase(pin, period, title, subtitle)
{
  _beta = DEFAULT_BETA;
  _maxVoltage = DEFAULT_MAX_VOLTAGE;
  _maxSamples = DEFAULT_MAX_SAMPLES;
  _nSamples = 0;
  _sumSamples = 0;
}

void ThermistorService::init()
{
  Serial.println("Criando o serviço " + String(getTitle().c_str()) + "...");
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &_calibration); //Inicializa a estrutura de calibracao

  EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(getTitle(), getSubtitle());

  _pCharacteristicValue = EasyBLE::createCharacteristic(
      pService,
      "THERMISTOR Value (ºC)",
      "Temperatura ambiente",
      EasyBLE::PROPERTY_OUTPUT,
      NULL);

  pService->start();

  Serial.println("Serviço " + String(getTitle().c_str()) + " criado.");
}

void ThermistorService::update()
{
  if (isReady())
  {
    // 1/Tk = 1/T25 + 1/B*ln(Vin/Vout - 1) [1/K]
    // Tc = 1/(1/Tk)-273.15 [C]
    const float zeroCelciusInKelvin = 273.15;
    _sumSamples += esp_adc_cal_raw_to_voltage(analogRead(ServiceBase<float>::getPin()), &_calibration);
    _nSamples++;
    if (_nSamples == _maxSamples)
    {
      float tInv = 1.0 / (25.0 + zeroCelciusInKelvin) + 2.3 / (float)_beta * log((float)_maxVoltage / ((float)_sumSamples / (float)_maxSamples) - 1.0);
      setState(1.0 / tInv - zeroCelciusInKelvin);
      publishState(_pCharacteristicValue);
      _nSamples = _sumSamples = 0;
    }
  }
}

void ThermistorService::setOptionals(unsigned short beta, unsigned short maxVoltage, unsigned short maxSamples)
{
  _beta = beta;
  _maxVoltage = maxVoltage;
  _maxSamples = maxSamples;
}