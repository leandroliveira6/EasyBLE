#include "EasyBLE.hpp"

using std::string;

int EasyBLE::_nServices;
bool EasyBLE::_deviceConnected;
BLEServer *EasyBLE::_PServer;

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

BLEServer *EasyBLE::getPServer(){
  if(EasyBLE::_PServer == NULL){
    EasyBLE::_nServices = 0;

    // Create the BLE Device
    BLEDevice::init("EasyBLE");
    
    // Create the BLE Server
    EasyBLE::_PServer = BLEDevice::createServer();
    EasyBLE::_PServer->setCallbacks(new MyServerCallbacks());

    // Start advertising
    EasyBLE::_PServer->getAdvertising()->start();
  }
  return EasyBLE::_PServer;  
}

bool EasyBLE::isConnected(){
  return EasyBLE::_deviceConnected;
}

void EasyBLE::changeConnection(bool newConnectionState){
  EasyBLE::_deviceConnected = newConnectionState;
}

BLEUUID EasyBLE::getNewUUID(){
  char newUUID[16];
  snprintf(newUUID, 16, "%015d0", ++EasyBLE::_nServices);
  return BLEUUID((uint8_t*)newUUID, 16, true);
}