#include "ReleService.hpp"

void releControlCallback(void *pObject, BLECharacteristic *pCharacteristic);

ReleService::ReleService(unsigned char pin) : ReleService::ReleService(pin, DEFAULT_PERIOD, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ReleService::ReleService(unsigned char pin, unsigned int period) : ReleService::ReleService(pin, period, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ReleService::ReleService(unsigned char pin, std::string title, std::string subtitle) : ReleService::ReleService(pin, DEFAULT_PERIOD, title, subtitle) {}
ReleService::ReleService(unsigned char pin, unsigned int period, std::string title, std::string subtitle) : ServiceBase::ServiceBase(pin, period, title, subtitle) {}

void ReleService::init()
{
  Serial.println("Criando o serviço " + String(getTitle().c_str()) + "...");
  pinMode(getPin(), OUTPUT);

  EasyBLE::createServer();
  BLEService *pService = EasyBLE::createService(getTitle(), getSubtitle());

  _pCharacteristicValue = EasyBLE::createCharacteristic(
      pService,
      "RELÉ State",
      "Exibição do estado do relé",
      EasyBLE::PROPERTY_OUTPUT,
      NULL);

  EasyBLE::createCharacteristic(
      pService,
      "RELÉ Control",
      "Controle do estado do relé",
      EasyBLE::PROPERTY_SWITCH,
      new EasyBLECharacteristicCallback(this, releControlCallback));

  pService->start();

  setState(STATE_OFF);

  Serial.println("Serviço " + String(getTitle().c_str()) + " criado.");
};

void ReleService::update()
{
  if (isReady(true))
  {
    publishState(_pCharacteristicValue);
    digitalWrite(getPin(), getState() == STATE_ON ? HIGH : LOW);
  }
};

BLECharacteristic *ReleService::getCharacteristicValue()
{
  return _pCharacteristicValue;
}

void releControlCallback(void *pObject, BLECharacteristic *pCharacteristic)
{
  ReleService *pReleService = (ReleService *)pObject;
  std::string value = EasyBLE::readValue(pCharacteristic);

  if (value.length() > 0)
  {
    Serial.println("Valor recebido: " + String(value.c_str()));
    pReleService->setState(value == "on" ? STATE_ON : STATE_OFF);
    pReleService->publishState(pReleService->getCharacteristicValue());
  }
}
