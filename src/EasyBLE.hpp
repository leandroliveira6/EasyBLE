#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
// #include <mutex>

#include "EasyBLECharacteristicCallback.hpp"

/**
 * @brief Modelo de EasyBLE.
 *
 */
class EasyBLE
{
  static unsigned short _nUuids;
  static bool _deviceConnected;
  static BLEServer *_pServer;
  // static std::mutex _mutex;

public:
  static const short int PROPERTY_INPUT = 0;  // recebe dados
  static const short int PROPERTY_OUTPUT = 1; // envia dados
  static const short int PROPERTY_SWITCH = 2; // liga ou desliga algo

  static BLEServer *createServer();
  static BLEService *createService(std::string name, std::string description);
  static BLECharacteristic *createCharacteristic(BLEService *pService, std::string name, std::string description, short int type, EasyBLECharacteristicCallback *callback);
  static std::string readValue(BLECharacteristic *pCharacteristic);
  static void writeValue(BLECharacteristic *pCharacteristic, std::string value);
  static void changeConnection(bool newConnectionState);
  static bool isConnected();
  static BLEUUID getNewUUID();
};
