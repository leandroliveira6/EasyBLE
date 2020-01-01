#include "LedService.hpp"

void ledControlCallback(void *pObject, BLECharacteristic *pCharacteristic);

LedService::LedService(unsigned char pin) : LedService::LedService(pin, DEFAULT_PERIOD, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
LedService::LedService(unsigned char pin, unsigned int period) : LedService::LedService(pin, period, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
LedService::LedService(unsigned char pin, std::string title, std::string subtitle) : LedService::LedService(pin, DEFAULT_PERIOD, title, subtitle) {}
LedService::LedService(unsigned char pin, unsigned int period, std::string title, std::string subtitle) : ServiceBase::ServiceBase(pin, period, title, subtitle) {}

void LedService::init()
{
  Serial.println("Criando o serviço " + String(getTitle().c_str()) + "...");
  pinMode(getPin(), OUTPUT);

  EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(getTitle(), getSubtitle());

  _pCharacteristicValue = EasyBLE::createCharacteristic(
      pService,
      "LED State",
      "Exibição do estado atual do LED",
      EasyBLE::PROPERTY_OUTPUT,
      NULL);

  EasyBLE::createCharacteristic(
      pService,
      "LED Control",
      "Controle do estado atual do LED",
      EasyBLE::PROPERTY_SWITCH,
      new EasyBLECharacteristicCallback(this, ledControlCallback));

  pService->start();

  setState(STATE_OFF);

  Serial.println("Serviço " + String(getTitle().c_str()) + " criado.");
};

void LedService::update()
{
  if (isReady(true))
  {
    publishState(_pCharacteristicValue);
    digitalWrite(getPin(), getState() == STATE_ON ? HIGH : LOW);
  }
}

BLECharacteristic *LedService::getCharacteristicValue()
{
  return _pCharacteristicValue;
}

void ledControlCallback(void *pObject, BLECharacteristic *pCharacteristic)
{
  LedService *pLedService = (LedService *)pObject;
  std::string value = EasyBLE::readValue(pCharacteristic);

  if (value.length() > 0)
  {
    Serial.println("Valor recebido: " + String(value.c_str()));
    pLedService->setState(value == "on" ? STATE_ON : STATE_OFF);
    pLedService->publishState(pLedService->getCharacteristicValue());
  }
}
