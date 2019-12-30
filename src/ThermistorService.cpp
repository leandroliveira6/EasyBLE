#include "ThermistorService.hpp"

ThermistorService::ThermistorService(int pin, int beta, std::string name, std::string description)
{
  _pin = pin;
  _name = name;
  _description = description;
  _beta = beta;
  _interval = 1000;
}

void ThermistorService::init()
{
  Serial.println("Criando o serviço " + String(_name.c_str()) + "...");

  EasyBLE::createServer();

  BLEService *pService = EasyBLE::createService(_name, _description);

  _pCharacteristicOutput = EasyBLE::createCharacteristic(pService, "THERMISTOR Value (ºC)", "Temperatura ambiente", EasyBLE::PROPERTY_OUTPUT, NULL);

  pService->start();

  Serial.println("Serviço " + String(_name.c_str()) + " criado.");
}

void ThermistorService::update()
{
  if (millis() - _lastMillis > _interval)
  {
    // 1/Tk = 1/T25 + 1/B*ln(Vin/Vout - 1) [1/K]
    // Tc = 1/(1/Tk)-273.15 [C]
    float zeroCelciusInKelvin = 273.15;
    float adcMax = 4095.0;
    float tInv = 1 / (25.0 + zeroCelciusInKelvin) + 2.3 / (float)_beta * log(adcMax / analogRead(39) - 1);
    changeState(1.0 / tInv - zeroCelciusInKelvin);
    publishState();
    _lastMillis = millis();
  }
}

void ThermistorService::changeState(float newState)
{
  _state = newState;
}

void ThermistorService::publishState()
{
  EasyBLE::writeValue(_pCharacteristicOutput, String(_state).c_str());
}