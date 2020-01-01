#include "ServiceBase.cpp"

class PhotoService : public ServiceBase<unsigned char>
{
  BLECharacteristic *_pCharacteristicValue;

public:
  PhotoService(unsigned char pin);
  PhotoService(unsigned char pin, unsigned int period);
  PhotoService(unsigned char pin, std::string title, std::string subtitle);
  PhotoService(unsigned char pin, unsigned int period, std::string title, std::string subtitle);
  void init();
  void update();
};
