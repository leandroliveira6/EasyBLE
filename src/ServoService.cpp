#include "ServoService.hpp"

void servoControlCallback(void *pObject, BLECharacteristic *pCharacteristic);

ServoService::ServoService(unsigned char pin) : ServoService::ServoService(pin, DEFAULT_PERIOD, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ServoService::ServoService(unsigned char pin, unsigned int period) : ServoService::ServoService(pin, period, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ServoService::ServoService(unsigned char pin, std::string title, std::string subtitle) : ServoService::ServoService(pin, DEFAULT_PERIOD, title, subtitle) {}
ServoService::ServoService(unsigned char pin, unsigned int period, std::string title, std::string subtitle) : ServiceBase::ServiceBase(pin, period, title, subtitle) {}

void ServoService::init()
{
  Serial.println("Criando o serviço " + String(getTitle().c_str()) + "...");
  _servo.setPeriodHertz(10); // Standard 50hz servo
  _servo.attach(getPin(), DEFAULT_MIN_PULSE, DEFAULT_MAX_PULSE);

  EasyBLE::createServer();
  BLEService *pService = EasyBLE::createService(getTitle(), getSubtitle());

  _pCharacteristicValue = EasyBLE::createCharacteristic(
      pService,
      "SERVO State",
      "Exibição do angulo atual do servo",
      EasyBLE::PROPERTY_OUTPUT,
      NULL);

  EasyBLE::createCharacteristic(
      pService,
      "SERVO Control",
      "Controle do angulo do servomotor. Os angulos estão limitados entre 0 e 180 graus",
      EasyBLE::PROPERTY_INPUT,
      new EasyBLECharacteristicCallback(this, servoControlCallback));

  pService->start();

  setState(0);

  Serial.println("Serviço " + String(getTitle().c_str()) + " criado.");
};

void ServoService::update()
{
  if (isReady(true))
  {
    publishState(_pCharacteristicValue);
    _servo.write(getState());
  }
};

void ServoService::setOptionals(unsigned short minPulseWidth, unsigned short maxPulseWidth)
{
  _servo.attach(getPin(), minPulseWidth, maxPulseWidth);
}

BLECharacteristic *ServoService::getCharacteristicValue()
{
  return _pCharacteristicValue;
}

void servoControlCallback(void *pObject, BLECharacteristic *pCharacteristic)
{
  ServoService *pServoService = (ServoService *)pObject;
  std::string value = EasyBLE::readValue(pCharacteristic);

  if (value.length() > 0)
  {
    Serial.println("Valor recebido: " + String(value.c_str()));
    pServoService->setState(constrain(String(value.c_str()).toInt(), 0, 180));
  }
}
