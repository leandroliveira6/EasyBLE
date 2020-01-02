#include <BLEServer.h>

/**
 * @brief Modelo de EasyBLEServerCallback.
 *
 */
class EasyBLEServerCallback : public BLEServerCallbacks
{
private:
  void onConnect(BLEServer *pServer) override;
  void onDisconnect(BLEServer *pServer) override;
};