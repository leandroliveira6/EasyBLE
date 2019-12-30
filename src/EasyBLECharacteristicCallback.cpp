#include "EasyBLECharacteristicCallback.hpp"

EasyBLECharacteristicCallback::EasyBLECharacteristicCallback(void *pObject, void (*pCallback)(void *pObject, BLECharacteristic *pCharacteristic))
{
  _pObject = pObject;
  _pCallback = pCallback;
}

void EasyBLECharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic)
{
  EasyBLECharacteristicCallback::_pCallback(_pObject, pCharacteristic);
}