#include <Arduino.h>
#include <BLEDevice.h>

#include "EasyBLECallback.hpp"

class EasyBLE{
  static unsigned int _nServices;
  static bool _deviceConnected;
  static BLEServer *_pServer;
  
  public:
  static const unsigned char 	PROPERTY_INPUT = 0; // recebe dados
  static const unsigned char 	PROPERTY_OUTPUT = 1; // envia dados
  static const unsigned char 	PROPERTY_SWITCH = 2; // liga ou desliga algo

  static BLEServer *createServer();
  static BLEService *createService(std::string name, std::string description);
  static BLECharacteristic *createCharacteristic(BLEService *pService, std::string name, std::string description, unsigned char type, EasyBLECallback *callback);
  static void changeConnection(bool newConnectionState);
  static bool isConnected();
  static BLEUUID getNewUUID();
};
