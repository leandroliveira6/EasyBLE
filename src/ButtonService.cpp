#include "ButtonService.hpp"

void buttonControlCallback(void *pObject, BLECharacteristic *pCharacteristic)
{
  ButtonService *pButtonService = (ButtonService *)pObject;
  std::string value = EasyBLE::readValue(pCharacteristic);

  if (value.length() > 0)
  {
    Serial.println("Valor recebido: " + String(value.c_str()));
    pButtonService->changeState(String(value.c_str()).toInt());
    pButtonService->publishState();
  }
}

// void IRAM_ATTR ButtonService::_buttonCallback()
// {
//   changeState(++_state);
//   publishState();
// }

ButtonService::ButtonService(int pin, std::string name, std::string description)
{
  _pin = pin;
  _name = name;
  _description = description;
  _state = 0;
  _interval = 140;
};

void IRAM_ATTR buttonCallback(void *pObject)
{
  ButtonService *pButtonService = (ButtonService *)pObject;
  int state = pButtonService->getState();
  pButtonService->changeState(++state);
  pButtonService->publishState();
}

void ButtonService::init()
{
  Serial.println("Criando o serviço " + String(_name.c_str()) + "...");
  // pinMode(_pin, INPUT);
  // attachInterrupt(_pin, _buttonCallback, FALLING);

  gpio_pad_select_gpio((gpio_num_t)_pin);
  gpio_set_direction((gpio_num_t)_pin, GPIO_MODE_INPUT);
  gpio_set_intr_type((gpio_num_t)_pin, GPIO_INTR_POSEDGE);
  gpio_install_isr_service(0);
  gpio_isr_handler_add((gpio_num_t)_pin, buttonCallback, this);

  EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(_name, _description);

  _pCharacteristicOutput = EasyBLE::createCharacteristic(pService, "BUTTON Click Count", "Exibe a quantidade de clicks no botão", EasyBLE::PROPERTY_OUTPUT, NULL);

  EasyBLE::createCharacteristic(
      pService,
      "BUTTON Click Control",
      "Controle do número de clicks exibido",
      EasyBLE::PROPERTY_INPUT,
      new EasyBLECharacteristicCallback(this, buttonControlCallback));

  pService->start();

  changeState(0);
  publishState();

  Serial.println("Serviço " + String(_name.c_str()) + " criado.");
};

void ButtonService::update()
{
}

void ButtonService::changeState(int newState)
{
  if (millis() - _lastMillis > _interval)
  {
    _state = newState;
    _lastMillis = millis();
  }
}

void ButtonService::publishState()
{
  EasyBLE::writeValue(_pCharacteristicOutput, String(_state).c_str());
}

int ButtonService::getState()
{
  return _state;
}
