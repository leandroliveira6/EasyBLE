#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>

class EasyBLE{
  static int _nServices;
  static bool _deviceConnected;
  static BLEServer *_PServer;
  
  public:
  static BLEServer *getPServer();
  static void changeConnection(bool newConnectionState);
  static bool isConnected();
  static BLEUUID getNewUUID();
};
