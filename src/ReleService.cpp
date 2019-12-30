#include "ReleService.hpp"

void releControlCallback(void *pObject, BLECharacteristic *pCharacteristic)
{
  ReleService *pReleService = (ReleService *)pObject;
  std::string value = EasyBLE::readValue(pCharacteristic);

  if (value.length() > 0)
  {
    Serial.println("Valor recebido: " + String(value.c_str()));

    if (value == "on")
    {
      pReleService->changeState(true);
    }
    else if (value == "off")
    {
      pReleService->changeState(false);
    }

    pReleService->publishState();
  }
}

ReleService::ReleService(int pin, std::string name, std::string description)
{
  _pin = pin;
  _name = name;
  _description = description;
  _state = false;
  _interval = 1000;
};

void ReleService::init()
{
  Serial.println("Criando o serviço " + String(_name.c_str()) + "...");
  pinMode(_pin, OUTPUT);

  EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(_name, _description);

  _pCharacteristicOutput = EasyBLE::createCharacteristic(pService, "RELÉ State", "Exibição do estado do relé", EasyBLE::PROPERTY_OUTPUT, NULL);

  EasyBLE::createCharacteristic(pService, "RELÉ Control", "Controle do estado do relé", EasyBLE::PROPERTY_SWITCH, new EasyBLECharacteristicCallback(this, releControlCallback));

  pService->start();

  changeState(false);
  publishState();

  Serial.println("Serviço " + String(_name.c_str()) + " criado.");
};

void ReleService::update()
{
  if (millis() - _lastMillis > _interval)
  {
    publishState();
    _lastMillis = millis();
  }
};

void ReleService::changeState(bool newState)
{
  _state = newState;
  if (_state)
  {
    digitalWrite(_pin, HIGH);
  }
  else
  {
    digitalWrite(_pin, LOW);
  }
}

void ReleService::publishState()
{
  if (_state)
  {
    EasyBLE::writeValue(_pCharacteristicOutput, "Ligado");
  }
  else
  {
    EasyBLE::writeValue(_pCharacteristicOutput, "Desligado");
  }
}
