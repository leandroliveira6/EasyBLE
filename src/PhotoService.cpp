#include "PhotoService.hpp"

PhotoService::PhotoService(int pin, std::string name, std::string description)
{
  _pin = pin;
  _name = name;
  _description = description;
  _interval = 1000;
}

void PhotoService::init()
{
  Serial.println("Criando o serviço " + String(_name.c_str()) + "...");

  EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(_name, _description);

  _pCharacteristicOutput = EasyBLE::createCharacteristic(pService, "PHOTO Value (%)", "Taxa de luminosidade do ambiente", EasyBLE::PROPERTY_OUTPUT, NULL);

  pService->start();

  Serial.println("Serviço " + String(_name.c_str()) + " criado.");
}

void PhotoService::update()
{
  if (millis() - _lastMillis > _interval)
  {
    int newState = map(analogRead(_pin), 0, 4095, 0, 100);
    changeState(newState);
    publishState();
    _lastMillis = millis();
  }
}

void PhotoService::changeState(int newState)
{
  _state = newState;
}

void PhotoService::publishState()
{
  EasyBLE::writeValue(_pCharacteristicOutput, String(_state).c_str());
}