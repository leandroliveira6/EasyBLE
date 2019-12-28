#include <BLECharacteristic.h>

class EasyBLECharacteristicCallback : public BLECharacteristicCallbacks
{
  void *_pObject;
  void (*_pCallback)(void *pObject, BLECharacteristic *pCharacteristic);

  void onWrite(BLECharacteristic *pCharacteristic) override;

public:
  EasyBLECharacteristicCallback(
      void *pObject,
      void (*pCallback)(void *pObject, BLECharacteristic *pCharacteristic));
};
