#include "PhotoService.hpp"

/**
 * @brief Construtores de PhotoService.
 *
 * Serviço de monitoramento de um photoresistor pelo aplicativo EasyBLE.
 * @param [in] pin Pino onde o componente está conectado. Apenas os pinos analógicos são compativeis.
 * @param [in] period Periodo (ms) de atualização das leituras e escritas. É opcional e possui como padrão 1000ms.
 * @param [in] title Titulo do serviço, exibido nos cards de serviço no aplicativo. É opcional e possui como padrão "Titulo não definido".
 * @param [in] subtitle Subtitulo do serviço, exibido nos cards de serviço no aplicativo. É opcional e possui como padrão "Subtitulo não definido".
 */
PhotoService::PhotoService(unsigned char pin) : PhotoService::PhotoService(pin, DEFAULT_PERIOD, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
PhotoService::PhotoService(unsigned char pin, unsigned int period) : PhotoService::PhotoService(pin, period, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
PhotoService::PhotoService(unsigned char pin, std::string title, std::string subtitle) : PhotoService::PhotoService(pin, DEFAULT_PERIOD, title, subtitle) {}
PhotoService::PhotoService(unsigned char pin, unsigned int period, std::string title, std::string subtitle) : ServiceBase::ServiceBase(pin, period, title, subtitle) {}

/**
 * @brief Inicializar o serviço.
 *
 * Método responsavel pela inicialização dos serviços, caracteristicas e descritores BLE.
 */
void PhotoService::init()
{
  Serial.print("Criando um PhotoService com o titulo: " + String(getTitle().c_str()) + "... ");

  // Cria um servidor BLE, caso o mesmo já não tenha sido criado.
  EasyBLE::createServer();

  // Cria um serviço BLE, único para cada modulo de serviço implementado.
  BLEService *pService = EasyBLE::createService(getTitle(), getSubtitle());

  // Cria uma caracteristica para atualização dos valores de estado exibidos no aplicativo.
  _pCharacteristicValue = EasyBLE::createCharacteristic(
      pService,
      "PHOTO Value (%)",
      "Taxa de luminosidade do ambiente",
      EasyBLE::PROPERTY_OUTPUT,
      NULL);

  // Inicia o servidor BLE
  pService->start();

  Serial.println("criado.");
}

/**
 * @brief Atualizar o serviço.
 *
 * Método responsavel pela atualização do estado no serviço e no aplicativo, periodicamente.
 */
void PhotoService::update()
{
  if (isReady())
  {
    setState(map(analogRead(getPin()), 0, 4095, 0, 100));
    publishState(_pCharacteristicValue);
  }
}
