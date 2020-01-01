#include "PotentiometerService.hpp"

PotentiometerService::PotentiometerService(unsigned char pin) : PotentiometerService::PotentiometerService(pin, DEFAULT_PERIOD, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
PotentiometerService::PotentiometerService(unsigned char pin, unsigned int period) : PotentiometerService::PotentiometerService(pin, period, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
PotentiometerService::PotentiometerService(unsigned char pin, std::string title, std::string subtitle) : PotentiometerService::PotentiometerService(pin, DEFAULT_PERIOD, title, subtitle) {}
PotentiometerService::PotentiometerService(unsigned char pin, unsigned int period, std::string title, std::string subtitle) : ServiceBase::ServiceBase(pin, period, title, subtitle) {}

void PotentiometerService::init()
{
  Serial.println("Criando o serviço " + String(getTitle().c_str()) + "...");

  EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(getTitle(), getSubtitle());

  _pCharacteristicValue = EasyBLE::createCharacteristic(
      pService,
      "POTENTIOMETER Value (%)",
      "Taxa de resistencia do potenciômetro",
      EasyBLE::PROPERTY_OUTPUT,
      NULL);

  pService->start();

  Serial.println("Serviço " + String(getTitle().c_str()) + " criado.");
}

void PotentiometerService::update()
{
  if (isReady())
  {
    setState(map(analogRead(getPin()), 0, 4095, 0, 100));
    publishState(_pCharacteristicValue);
  }
}
