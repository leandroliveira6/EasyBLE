#include <BLEServer.h>

/**
 * @brief Modelo do EasyBLEServerCallback.
 *
 */
class EasyBLEServerCallback : public BLEServerCallbacks
{
private:
  void onConnect(BLEServer *pServer) override;
  void onDisconnect(BLEServer *pServer) override;
};