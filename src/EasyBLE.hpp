#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
// #include <mutex>

#include "EasyBLECharacteristicCallback.hpp"

/**
 * @brief Modelo do EasyBLE.
 *
 */
class EasyBLE
{
public:
  static const unsigned char PROPERTY_INPUT = 0;  // recebe dados
  static const unsigned char PROPERTY_OUTPUT = 1; // envia dados
  static const unsigned char PROPERTY_SWITCH = 2; // liga ou desliga algo

  static BLEServer *createServer();
  static BLEService *createService(std::string title, std::string subtitle);
  static BLECharacteristic *createCharacteristic(BLEService *pService, std::string title, std::string subtitle, unsigned char type, EasyBLECharacteristicCallback *callback);
  static std::string readValue(BLECharacteristic *pCharacteristic);
  static void writeValue(BLECharacteristic *pCharacteristic, std::string value);
  static void changeConnection(bool newConnection);
  static bool isConnected();
  static BLEUUID getNewUUID();

private:
  static unsigned short _nUuids;
  static bool _deviceConnected;
  static BLEServer *_pServer;
  // static std::mutex _mutex;
};
