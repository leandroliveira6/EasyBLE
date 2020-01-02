#include <BLECharacteristic.h>

/**
 * @brief Modelo do EasyBLECharacteristicCallback.
 *
 */
class EasyBLECharacteristicCallback : public BLECharacteristicCallbacks
{
public:
  EasyBLECharacteristicCallback(void *pObject, void (*pCallback)(void *pObject, BLECharacteristic *pCharacteristic));

private:
  void *_pObject;
  void (*_pCallback)(void *pObject, BLECharacteristic *pCharacteristic);

  void onWrite(BLECharacteristic *pCharacteristic) override;
};
