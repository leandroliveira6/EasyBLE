#include "LedService.hpp"

BLECharacteristic *pCharacteristicTX;

void _ledController(BLECharacteristic *pCharacteristic, int pin) {
    std::string rxValue = pCharacteristic->getValue();
    if (rxValue.length() > 0) {
      Serial.print("Valor recebido para o pino " + String(pin) + ": ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }
      Serial.println();

      if(rxValue == "on"){
        digitalWrite(pin, HIGH);
      }else if(rxValue == "off"){
        digitalWrite(pin, LOW);
      }
    }
  }

LedService::LedService(int pin, std::string name, std::string description){
    _pin = pin;
    _name = name;
    _description = description;
};

void LedService::init(){
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);

  BLEServer *pServer = EasyBLE::createServer();
  BLEService *pService = EasyBLE::createService(_name, _description);
  pCharacteristicTX = EasyBLE::createCharacteristic(pService, "Pino", "Pino do LED", EasyBLE::PROPERTY_OUTPUT, NULL);
  EasyBLE::createCharacteristic(pService, "Controle", "Controle do LED", EasyBLE::PROPERTY_INPUT, new EasyBLECallback(_pin, NULL, _ledController));

  pService->start();
  Serial.println("Serviço (Pino: "+String(_pin)+") iniciado!");
};

void LedService::update(){
  if (EasyBLE::isConnected()) {
    // Fabricate some arbitrary junk for now...
    //txValue = analogRead(readPin) / 3.456; // This could be an actual sensor reading!
    // Let's convert the value to a char array:
    char txString[8]; // make sure this is big enuffz
    dtostrf(_pin, 1, 0, txString); // float_val, min_width, digits_after_decimal, char_buffer
    //pCharacteristic->setValue(&txValue, 1); // To send the integer value
    //pCharacteristic->setValue("Hello!"); // Sending a test message
    pCharacteristicTX->setValue(txString);
    pCharacteristicTX->notify();
  }
};