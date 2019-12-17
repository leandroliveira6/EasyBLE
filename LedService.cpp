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

      if(rxValue == "ledon"){
        digitalWrite(pin, HIGH);
      }else if(rxValue == "ledoff"){
        digitalWrite(pin, LOW);
      }
    }
  }

LedService::LedService(int pin){
    _pin = pin;
};

void LedService::init(){
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);

  int nServices = 0;

  BLEServer *pServer = EasyBLE::createServer();

  // Create the BLE Service
  BLEService *pService = EasyBLE::createService();

  // Create a BLE Characteristics
  pCharacteristicTX = EasyBLE::createCharacteristic(pService, "Teste do TX", NULL, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

  BLECharacteristic *pCharacteristicRX = EasyBLE::createCharacteristic(pService, "Teste do RX",  new EasyBLECallback(_pin, NULL, _ledController), BLECharacteristic::PROPERTY_WRITE);
  
  // Start the service
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