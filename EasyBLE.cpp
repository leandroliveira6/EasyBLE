#include "EasyBLE.hpp"
#include "EasyBLEServerCallback.hpp"

unsigned int EasyBLE::_nServices;
bool EasyBLE::_deviceConnected;
BLEServer *EasyBLE::_pServer;

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Dispositivo conectado!");
    EasyBLE::changeConnection(true);
  };

  void onDisconnect(BLEServer* pServer) {
    Serial.println("Dispositivo desconectado!");
    EasyBLE::changeConnection(false);
  }
};

BLEServer *EasyBLE::createServer(){
  if(EasyBLE::_pServer == NULL){
    EasyBLE::_nServices = 0;

    // Create the BLE Device
    BLEDevice::init("EasyBLE");
    
    // Create the BLE Server
    EasyBLE::_pServer = BLEDevice::createServer();
    EasyBLE::_pServer->setCallbacks(new EasyBLEServerCallback());

    // Start advertising
    EasyBLE::_pServer->startAdvertising();
  }
  return EasyBLE::_pServer;  
}

BLEService *EasyBLE::createService(){
  BLEService *pService = EasyBLE::_pServer->createService(EasyBLE::getNewUUID());
  return pService;
}

BLECharacteristic *EasyBLE::createCharacteristic(BLEService *pService, std::string descritor, EasyBLECallback *callback, unsigned int properties){
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(EasyBLE::getNewUUID(), properties);
  BLEDescriptor *pDescriptor = new BLEDescriptor(EasyBLE::getNewUUID());
  pDescriptor->setValue(descritor);
  pCharacteristic->addDescriptor(pDescriptor);
  if(callback != NULL){
    pCharacteristic->setCallbacks(callback);
  }
  return pCharacteristic;
}

bool EasyBLE::isConnected(){
  return EasyBLE::_deviceConnected;
}

void EasyBLE::changeConnection(bool newConnectionState){
  EasyBLE::_deviceConnected = newConnectionState;
  if(EasyBLE::isConnected()){
    Serial.println("Dispositivo conectado!");
  }
  else{
    Serial.println("Dispositivo desconectado!");
  }
}

BLEUUID EasyBLE::getNewUUID(){
  return BLEUUID(++EasyBLE::_nServices);
}