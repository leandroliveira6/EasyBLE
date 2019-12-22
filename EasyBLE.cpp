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

void addServiceDescriptors(BLEService *pService, std::string name, std::string description){
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(EasyBLE::getNewUUID(), BLECharacteristic::PROPERTY_READ);
  std::string descriptions[] = {name, description};
  for(std::string description: descriptions){
    BLEDescriptor *pDescriptor = new BLEDescriptor(EasyBLE::getNewUUID());
    pDescriptor->setValue(description);
    pCharacteristic->addDescriptor(pDescriptor);
  }
}

BLEService *EasyBLE::createService(std::string name, std::string description){
  BLEService *pService = EasyBLE::_pServer->createService(EasyBLE::getNewUUID());
  addServiceDescriptors(pService, name, description);
  return pService;
}

void addCharacteristicDescriptors(BLECharacteristic *pCharacteristic, std::string name, std::string description, unsigned char type){
  std::string descriptions[] = {name, description};
  for(std::string description: descriptions){
    BLEDescriptor *pDescriptor = new BLEDescriptor(EasyBLE::getNewUUID());
    pDescriptor->setValue(description);
    pCharacteristic->addDescriptor(pDescriptor);
  }

  if(type == EasyBLE::PROPERTY_INPUT || type == EasyBLE::PROPERTY_SWITCH){
    pCharacteristic->setWriteProperty(true);
  } else if(type == EasyBLE::PROPERTY_OUTPUT){
    pCharacteristic->setReadProperty(true);
    pCharacteristic->setNotifyProperty(true);
  }
}

BLECharacteristic *EasyBLE::createCharacteristic(BLEService *pService, std::string name, std::string description, unsigned char type, EasyBLECallback *callback){
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(EasyBLE::getNewUUID(), 0);
  addCharacteristicDescriptors(pCharacteristic, name, description, type);
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