#include "EasyBLECharacteristicCallback.hpp"

/**
 * @brief Construtor de EasyBLECharacteristicCallback.
 *
 * Classe para os callbacks usados nas caracteristicas de um serviço. Armazena a referencia da instancia do serviço 
 * da caracteristica e aciona a função callback sempre que uma escrita é realizada na caracteristica em questão.
 * @param [in] pObject Referencia do serviço da caracteristica.
 * @param [in] pCallback Função callback chamada a cada escrita na caracteristica.
 */
EasyBLECharacteristicCallback::EasyBLECharacteristicCallback(void *pObject, void (*pCallback)(void *pObject, BLECharacteristic *pCharacteristic))
{
  _pObject = pObject;
  _pCallback = pCallback;
}

void EasyBLECharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic)
{
  EasyBLECharacteristicCallback::_pCallback(_pObject, pCharacteristic);
}