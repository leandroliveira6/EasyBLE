#include "LedService.hpp"

#define SERVICE_UUID           "6E400004-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400011-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400012-B5A3-F393-E0A9-E50E24DCCA9E"

using std::string;

BLECharacteristic *pCharacteristicTX;
float txValue = 0;
const int readPin = 32; // Use GPIO number. See ESP32 board pinouts
bool isEnabled;

class MyCallbacks: public BLECharacteristicCallbacks {
  int _pin;

  void onWrite(BLECharacteristic *pCharacteristic) {
    string rxValue = pCharacteristic->getValue();
    if (rxValue.length() > 0) {
      Serial.print("Valor recebido para o pino " + String(_pin) + ": ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }
      Serial.println();

      if(rxValue == "ledon"){
        digitalWrite(_pin, HIGH);
      }else if(rxValue == "ledoff"){
        digitalWrite(_pin, LOW);
      }
    }
  }

  public:
  MyCallbacks(int pin){
    _pin = pin;
  }
};

LedService::LedService(int pin){
    _pin = pin;
};

void LedService::init(){
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);

  int nServices = 0;

  BLEServer *pServer = EasyBLE::getPServer();

  // Create the BLE Service
  BLEService *pService = pServer->createService(EasyBLE::getNewUUID());

  // Create a BLE Characteristic
  pCharacteristicTX = pService->createCharacteristic(EasyBLE::getNewUUID(), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);//TX
  pCharacteristicTX->addDescriptor(new BLE2902());

  BLECharacteristic *pCharacteristicRX = pService->createCharacteristic(EasyBLE::getNewUUID(), BLECharacteristic::PROPERTY_WRITE);//RX
  pCharacteristicRX->setCallbacks(new MyCallbacks(_pin));

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