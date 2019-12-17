#include <BLECharacteristic.h>

class EasyBLECallback: public BLECharacteristicCallbacks{
  int _pin;
  void (*_pReadCallback)(BLECharacteristic *pCharacteristic, int pin);
  void (*_pWriteCallback)(BLECharacteristic *pCharacteristic, int pin);

  void onRead(BLECharacteristic *pCharacteristic) override;
  void onWrite(BLECharacteristic *pCharacteristic) override;
  
  public:
  EasyBLECallback(int pin, void (*pReadCallback)(BLECharacteristic *pCharacteristic, int pin), void (*pWriteCallback)(BLECharacteristic *pCharacteristic, int pin));
};
