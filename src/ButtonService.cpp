#include "ButtonService.hpp"

void buttonControlCallback(void *pObject, BLECharacteristic *pCharacteristic);
void IRAM_ATTR buttonCallback(void *pObject);

ButtonService::ButtonService(unsigned char pin) : ButtonService::ButtonService(pin, DEFAULT_PERIOD, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ButtonService::ButtonService(unsigned char pin, unsigned int period) : ButtonService::ButtonService(pin, period, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ButtonService::ButtonService(unsigned char pin, std::string title, std::string subtitle) : ButtonService::ButtonService(pin, DEFAULT_PERIOD, title, subtitle) {}
ButtonService::ButtonService(unsigned char pin, unsigned int period, std::string title, std::string subtitle) : ServiceBase::ServiceBase(pin, period, title, subtitle) {}

void ButtonService::init()
{
  Serial.println("Criando o serviço " + String(getTitle().c_str()) + "...");
  gpio_set_direction((gpio_num_t)getPin(), GPIO_MODE_INPUT);
  gpio_set_intr_type((gpio_num_t)getPin(), GPIO_INTR_NEGEDGE);
  gpio_install_isr_service(0);
  gpio_isr_handler_add((gpio_num_t)getPin(), buttonCallback, this);

  EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(getTitle(), getSubtitle());

  _pCharacteristicValue = EasyBLE::createCharacteristic(
      pService,
      "BUTTON Click Count",
      "Exibe a quantidade de clicks no botão",
      EasyBLE::PROPERTY_OUTPUT,
      NULL);

  EasyBLE::createCharacteristic(
      pService,
      "BUTTON Click Control",
      "Controle do número de clicks exibido",
      EasyBLE::PROPERTY_INPUT,
      new EasyBLECharacteristicCallback(this, buttonControlCallback));

  pService->start();

  setState(0);

  Serial.println("Serviço " + String(getTitle().c_str()) + " criado.");
};

void ButtonService::update()
{
  if (isReady())
  {
    publishState(_pCharacteristicValue);
  }
}

BLECharacteristic *ButtonService::getCharacteristicValue()
{
  return _pCharacteristicValue;
}

void buttonControlCallback(void *pObject, BLECharacteristic *pCharacteristic)
{
  ButtonService *pButtonService = (ButtonService *)pObject;
  std::string value = EasyBLE::readValue(pCharacteristic);

  if (value.length() > 0)
  {
    Serial.println("Valor recebido: " + String(value.c_str()));
    pButtonService->setState(String(value.c_str()).toInt());
    pButtonService->publishState(pButtonService->getCharacteristicValue());
  }
}

void IRAM_ATTR buttonCallback(void *pObject)
{
  ButtonService *pButtonService = (ButtonService *)pObject;
  int state = pButtonService->getState();
  pButtonService->setState(++state);
  pButtonService->publishState(pButtonService->getCharacteristicValue());
}