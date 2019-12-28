#include "PotentiometerService.hpp"

PotentiometerService::PotentiometerService(int pin, std::string name, std::string description)
{
  _pin = pin;
  _name = name;
  _description = description;
}

void PotentiometerService::init()
{
  Serial.println("Criando o serviço " + String(_name.c_str()) + "...");

  BLEServer *pServer = EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(_name, _description);

  _pCharacteristicOutput = EasyBLE::createCharacteristic(pService, "POTENTIOMETER Value (%)", "Taxa de resistencia do potenciômetro", EasyBLE::PROPERTY_OUTPUT, NULL);

  pService->start();

  Serial.println("Serviço " + String(_name.c_str()) + " criado.");
}

void PotentiometerService::update()
{
  //Serial.println(String(map(analogRead(_pin), 0, 1024, 0, 100)).c_str());
  EasyBLE::writeValue(_pCharacteristicOutput, String(map(analogRead(_pin), 0, 4095, 0, 100)).c_str());
}