#include "EasyBLECallback.hpp"

EasyBLECallback::EasyBLECallback(int pin, void (*pReadCallback)(BLECharacteristic *pCharacteristic, int pin), void (*pWriteCallback)(BLECharacteristic *pCharacteristic, int pin)){
    _pin = pin;
    _pReadCallback = pReadCallback;
    _pWriteCallback = pWriteCallback;
}

void EasyBLECallback::onRead(BLECharacteristic *pCharacteristic){
    EasyBLECallback::_pReadCallback(pCharacteristic, _pin);
}

void EasyBLECallback::onWrite(BLECharacteristic *pCharacteristic){
    EasyBLECallback::_pWriteCallback(pCharacteristic, _pin);
}