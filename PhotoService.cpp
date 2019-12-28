#include "PhotoService.hpp"

PhotoService::PhotoService(int pin, std::string name, std::string description)
{
  _pin = pin;
  _name = name;
  _description = description;
}

void PhotoService::init()
{
  Serial.println("Criando o serviço " + String(_name.c_str()) + "...");

  BLEServer *pServer = EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(_name, _description);

  _pCharacteristicOutput = EasyBLE::createCharacteristic(pService, "PHOTO Value (%)", "Taxa de luminosidade do ambiente", EasyBLE::PROPERTY_OUTPUT, NULL);

  pService->start();

  Serial.println("Serviço " + String(_name.c_str()) + " criado.");
}

void PhotoService::update()
{
  EasyBLE::writeValue(_pCharacteristicOutput, String(map(analogRead(_pin), 0, 4095, 0, 100)).c_str());
}