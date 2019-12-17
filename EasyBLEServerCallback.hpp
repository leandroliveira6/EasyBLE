#include <BLEServer.h>

class EasyBLEServerCallback: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override;
  void onDisconnect(BLEServer* pServer) override;
};