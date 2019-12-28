#include "EasyBLE.hpp"
#include "EasyBLEServerCallback.hpp"

unsigned int EasyBLE::_nServices;
bool EasyBLE::_deviceConnected;
BLEServer *EasyBLE::_pServer;
std::mutex EasyBLE::_mutex;

BLEServer *EasyBLE::createServer()
{
  if (EasyBLE::_pServer == NULL)
  {
    EasyBLE::_nServices = 0;

    BLEDevice::init("EasyBLE");

    EasyBLE::_pServer = BLEDevice::createServer();
    EasyBLE::_pServer->setCallbacks(new EasyBLEServerCallback());

    EasyBLE::_pServer->startAdvertising();
  }
  return EasyBLE::_pServer;
}

std::string checkText(std::string text)
{
  if (!text.empty())
  {
    if (text.length() > 100)
    {
      return text.substr(0, 100);
    }
    return text;
  }
  return "Valor não informado";
}

void addCharacteristicDetails(BLECharacteristic *pCharacteristic, std::string name, std::string description)
{
  std::string values[] = {name, description};

  for (std::string value : values)
  {
    BLEDescriptor *pDescriptor = new BLEDescriptor(EasyBLE::getNewUUID());
    pDescriptor->setValue(checkText(value));
    pCharacteristic->addDescriptor(pDescriptor);
  }
}

BLEService *EasyBLE::createService(std::string name, std::string description)
{
  BLEService *pService = EasyBLE::_pServer->createService(EasyBLE::getNewUUID());
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(EasyBLE::getNewUUID(), BLECharacteristic::PROPERTY_READ);
  addCharacteristicDetails(pCharacteristic, name, description);
  return pService;
}

BLECharacteristic *EasyBLE::createCharacteristic(BLEService *pService, std::string name, std::string description, short int type, EasyBLECharacteristicCallback *callback)
{
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(EasyBLE::getNewUUID(), 0);
  addCharacteristicDetails(pCharacteristic, name, description);

  if (type == EasyBLE::PROPERTY_INPUT)
  {
    pCharacteristic->setWriteNoResponseProperty(true);
  }
  else if (type == EasyBLE::PROPERTY_SWITCH)
  {
    pCharacteristic->setWriteProperty(true);
  }
  else if (type == EasyBLE::PROPERTY_OUTPUT)
  {
    pCharacteristic->setReadProperty(true);
    pCharacteristic->setNotifyProperty(true);
  }

  if (callback != NULL)
  {
    pCharacteristic->setCallbacks(callback);
  }
  return pCharacteristic;
}

bool EasyBLE::isConnected()
{
  return EasyBLE::_deviceConnected;
}

void EasyBLE::changeConnection(bool newConnectionState)
{
  EasyBLE::_deviceConnected = newConnectionState;
  if (EasyBLE::isConnected())
  {
    Serial.println("Um dispositivo foi conectado.");
  }
  else
  {
    Serial.println("Um dispositivo foi desconectado");
  }
}

std::string EasyBLE::readValue(BLECharacteristic *pCharacteristic)
{
  if (EasyBLE::isConnected())
  {
    std::string value;
    //EasyBLE::_mutex.lock();
    value = pCharacteristic->getValue();
    //EasyBLE::_mutex.unlock();
    return value;
  }
}

void EasyBLE::writeValue(BLECharacteristic *pCharacteristic, std::string value)
{
  if (EasyBLE::isConnected())
  {
    //EasyBLE::_mutex.lock();
    pCharacteristic->setValue(value);
    pCharacteristic->notify();
    //EasyBLE::_mutex.unlock();
  }
}

BLEUUID EasyBLE::getNewUUID()
{
  return BLEUUID(++EasyBLE::_nServices);
}