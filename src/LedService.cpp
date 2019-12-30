#include "LedService.hpp"

void ledControlCallback(void *pObject, BLECharacteristic *pCharacteristic)
{
  LedService *pLedService = (LedService *)pObject;
  std::string value = EasyBLE::readValue(pCharacteristic);

  if (value.length() > 0)
  {
    Serial.println("Valor recebido: " + String(value.c_str()));

    if (value == "on")
    {
      pLedService->changeState(true);
    }
    else if (value == "off")
    {
      pLedService->changeState(false);
    }

    pLedService->publishState();
  }
}

LedService::LedService(int pin, std::string name, std::string description)
{
  _pin = pin;
  _name = name;
  _description = description;
  _state = false;
  _interval = 1000;
};

void LedService::init()
{
  Serial.println("Criando o serviço " + String(_name.c_str()) + "...");
  pinMode(_pin, OUTPUT);

  EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(_name, _description);

  _pCharacteristicOutput = EasyBLE::createCharacteristic(pService, "LED State", "Exibição do estado atual do LED", EasyBLE::PROPERTY_OUTPUT, NULL);

  EasyBLE::createCharacteristic(pService, "LED Control", "Controle do estado atual do LED", EasyBLE::PROPERTY_SWITCH, new EasyBLECharacteristicCallback(this, ledControlCallback));

  pService->start();

  changeState(false);
  publishState();

  Serial.println("Serviço " + String(_name.c_str()) + " criado.");
};

void LedService::update()
{
  if (millis() - _lastMillis > _interval)
  {
    publishState();
    _lastMillis = millis();
  }
};

void LedService::changeState(bool newState)
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

void LedService::publishState()
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
