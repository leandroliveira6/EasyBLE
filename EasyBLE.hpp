#include <Arduino.h>
#include <BLEDevice.h>

#include "EasyBLECallback.hpp"

class EasyBLE{
  static unsigned int _nServices;
  static bool _deviceConnected;
  static BLEServer *_pServer;
  
  public:
  static BLEServer *createServer();
  static BLEService *createService();
  static BLECharacteristic *createCharacteristic(BLEService *pService, std::string descritor, EasyBLECallback *callback, unsigned int properties);
  static void changeConnection(bool newConnectionState);
  static bool isConnected();
  static BLEUUID getNewUUID();
};
