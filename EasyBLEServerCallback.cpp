#include "EasyBLEServerCallback.hpp"
#include "EasyBLE.hpp"

void EasyBLEServerCallback::onConnect(BLEServer *pServer)
{
    EasyBLE::changeConnection(true);
};

void EasyBLEServerCallback::onDisconnect(BLEServer *pServer)
{
    EasyBLE::changeConnection(false);
}