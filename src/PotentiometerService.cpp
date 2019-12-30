#include "PotentiometerService.hpp"

PotentiometerService::PotentiometerService(int pin, std::string name, std::string description)
{
  _pin = pin;
  _name = name;
  _description = description;
  _interval = 1000;
}

void PotentiometerService::init()
{
  Serial.println("Criando o serviço " + String(_name.c_str()) + "...");

  EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(_name, _description);

  _pCharacteristicOutput = EasyBLE::createCharacteristic(pService, "POTENTIOMETER Value (%)", "Taxa de resistencia do potenciômetro", EasyBLE::PROPERTY_OUTPUT, NULL);

  pService->start();

  Serial.println("Serviço " + String(_name.c_str()) + " criado.");
}

void PotentiometerService::update()
{
  if (millis() - _lastMillis > _interval)
  {
    int newState = map(analogRead(_pin), 0, 4095, 0, 100);
    changeState(newState);
    publishState();
    _lastMillis = millis();
  }
}

void PotentiometerService::changeState(int newState)
{
  _state = newState;
}

void PotentiometerService::publishState()
{
  EasyBLE::writeValue(_pCharacteristicOutput, String(_state).c_str());
}