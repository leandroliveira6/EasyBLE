#include "ServoService.hpp"

void servoControlCallback(void *pObject, BLECharacteristic *pCharacteristic)
{
  ServoService *pServoService = (ServoService *)pObject;
  std::string value = EasyBLE::readValue(pCharacteristic);

  if (value.length() > 0)
  {
    Serial.println("Valor recebido: " + String(value.c_str()));
    pServoService->changeState(String(value.c_str()).toInt());
    pServoService->publishState();
  }
}

ServoService::ServoService(int pin, std::string name, std::string description)
{
  _pin = pin;
  _name = name;
  _description = description;
  _interval = 1000;
};

void ServoService::init()
{
  Serial.println("Criando o serviço " + String(_name.c_str()) + "...");
  _servo.setPeriodHertz(200); // Standard 50hz servo
  _servo.attach(_pin, 500, 2400);

  EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(_name, _description);

  _pCharacteristicOutput = EasyBLE::createCharacteristic(pService, "SERVO State", "Exibição do angulo atual do servo", EasyBLE::PROPERTY_OUTPUT, NULL);

  EasyBLE::createCharacteristic(
      pService,
      "SERVO Control",
      "Controle do angulo do servomotor. Os angulos estão limitados entre 0 e 180 graus",
      EasyBLE::PROPERTY_INPUT,
      new EasyBLECharacteristicCallback(this, servoControlCallback));

  pService->start();

  changeState(0);
  publishState();

  Serial.println("Serviço " + String(_name.c_str()) + " criado.");
};

void ServoService::update()
{
  if (millis() - _lastMillis > _interval)
  {
    publishState();
    _lastMillis = millis();
  }
};

void ServoService::changeState(int newState)
{
  _state = constrain(newState, 0, 180);
  _servo.write(_state);
}

void ServoService::publishState()
{
  EasyBLE::writeValue(_pCharacteristicOutput, String(_state).c_str());
}
