#ifndef SERVICE_BASE
#define SERVICE_BASE

#include "EasyBLE.hpp"

#define DEFAULT_PERIOD 1000
#define DEFAULT_TITLE "Titulo não definido"
#define DEFAULT_SUBTITLE "Subtitulo não definido"

/**
 * @brief Modelo de ServiceBase.
 *
 */
template <class T>
class ServiceBase
{
  unsigned char _pin;
  std::string _title, _subtitle;
  unsigned int _period;
  unsigned long _lastMillis;
  T _state;
  T _lastState;

public:
  ServiceBase(unsigned char pin, unsigned int period, std::string name, std::string description);
  unsigned char getPin();
  std::string getTitle();
  std::string getSubtitle();
  T getState();
  void setState(T newState);
  bool isReady(bool priority = false);
  void publishState(BLECharacteristic *pCharacteristic);
};

template <class T>
ServiceBase<T>::ServiceBase(unsigned char pin, unsigned int period, std::string title, std::string subtitle)
{
  _pin = pin;
  _title = title;
  _subtitle = subtitle;
  _period = period;
  _lastMillis = millis();
}

template <class T>
unsigned char ServiceBase<T>::getPin()
{
  return _pin;
}

template <class T>
std::string ServiceBase<T>::getTitle()
{
  return _title;
}

template <class T>
std::string ServiceBase<T>::getSubtitle()
{
  return _subtitle;
}

template <class T>
T ServiceBase<T>::getState()
{
  return _state;
}

template <class T>
void ServiceBase<T>::setState(T newState)
{
  _lastState = _state;
  _state = newState;
}

template <class T>
bool ServiceBase<T>::isReady(bool priority)
{
  if ((millis() - _lastMillis) > _period || (_lastState != _state && priority))
  {
    _lastMillis = millis();
    _lastState = _state;
    return true;
  }
  return false;
}

template <class T>
void ServiceBase<T>::publishState(BLECharacteristic *pCharacteristic)
{
  EasyBLE::writeValue(pCharacteristic, String(_state).c_str());
}

#endif