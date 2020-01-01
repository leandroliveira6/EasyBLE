#include "PhotoService.hpp"

PhotoService::PhotoService(unsigned char pin) : PhotoService::PhotoService(pin, DEFAULT_PERIOD, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
PhotoService::PhotoService(unsigned char pin, unsigned int period) : PhotoService::PhotoService(pin, period, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
PhotoService::PhotoService(unsigned char pin, std::string title, std::string subtitle) : PhotoService::PhotoService(pin, DEFAULT_PERIOD, title, subtitle) {}
PhotoService::PhotoService(unsigned char pin, unsigned int period, std::string title, std::string subtitle) : ServiceBase::ServiceBase(pin, period, title, subtitle) {}

void PhotoService::init()
{
  Serial.println("Criando o serviço " + String(getTitle().c_str()) + "...");

  EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(getTitle(), getSubtitle());

  _pCharacteristicValue = EasyBLE::createCharacteristic(
      pService,
      "PHOTO Value (%)",
      "Taxa de luminosidade do ambiente",
      EasyBLE::PROPERTY_OUTPUT,
      NULL);

  pService->start();

  Serial.println("Serviço " + String(getTitle().c_str()) + " criado.");
}

void PhotoService::update()
{
  if (isReady())
  {
    setState(map(analogRead(getPin()), 0, 4095, 0, 100));
    publishState(_pCharacteristicValue);
  }
}
