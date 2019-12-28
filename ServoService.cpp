#include "ServoService.hpp"

void servoControlCallback(void *pObject, BLECharacteristic *pCharacteristic)
{
  ServoService *pServoService = (ServoService *)pObject;
  std::string value = EasyBLE::readValue(pCharacteristic);

  if (value.length() > 0)
  {
    Serial.println("Valor recebido: " + String(value.c_str()));
    pServoService->changePosition(String(value.c_str()).toInt());
    pServoService->publishPosition();
  }
}

ServoService::ServoService(int pin, std::string name, std::string description)
{
  _pin = pin;
  _name = name;
  _description = description;
  _position = 0;
  //_pServo = new Servo();
};

void ServoService::init()
{
  Serial.println("Criando o serviço " + String(_name.c_str()) + "...");
  _servo.setPeriodHertz(200); // Standard 50hz servo
  _servo.attach(_pin, 500, 2400);

  BLEServer *pServer = EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(_name, _description);

  _pCharacteristicOutput = EasyBLE::createCharacteristic(pService, "SERVO State", "Exibição do angulo atual do servo", EasyBLE::PROPERTY_OUTPUT, NULL);

  EasyBLE::createCharacteristic(
      pService,
      "SERVO Control",
      "Controle do angulo do servomotor. Os angulos estão limitados entre 0 e 180 graus",
      EasyBLE::PROPERTY_INPUT,
      new EasyBLECharacteristicCallback(this, servoControlCallback)
  );

  pService->start();

  changePosition(0);
  publishPosition();

  Serial.println("Serviço " + String(_name.c_str()) + " criado.");
};

void ServoService::update()
{
  publishPosition();
};

void ServoService::changePosition(int newPosition)
{
  _position = newPosition;
  _position = constrain(_position, 0, 180);
  _servo.write(_position);
}

void ServoService::publishPosition()
{
  EasyBLE::writeValue(_pCharacteristicOutput, String(_position).c_str());
}
