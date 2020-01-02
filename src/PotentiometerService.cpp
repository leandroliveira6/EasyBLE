#include "PotentiometerService.hpp"

/**
 * @brief Construtores de PotentiometerService.
 *
 * Serviço de monitoramento de um potenciometro pelo aplicativo EasyBLE.
 * @param [in] pin Pino onde o componente está conectado. Apenas os pinos analógicos são compativeis.
 * @param [in] period Periodo (ms) de atualização das leituras e escritas. É opcional e possui como padrão 1000ms.
 * @param [in] title Titulo do serviço, exibido nos cards de serviço no aplicativo. É opcional e possui como padrão "Titulo não definido".
 * @param [in] subtitle Subtitulo do serviço, exibido nos cards de serviço no aplicativo. É opcional e possui como padrão "Subtitulo não definido".
 */
PotentiometerService::PotentiometerService(unsigned char pin) : PotentiometerService::PotentiometerService(pin, DEFAULT_PERIOD, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
PotentiometerService::PotentiometerService(unsigned char pin, unsigned int period) : PotentiometerService::PotentiometerService(pin, period, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
PotentiometerService::PotentiometerService(unsigned char pin, std::string title, std::string subtitle) : PotentiometerService::PotentiometerService(pin, DEFAULT_PERIOD, title, subtitle) {}
PotentiometerService::PotentiometerService(unsigned char pin, unsigned int period, std::string title, std::string subtitle) : ServiceBase::ServiceBase(pin, period, title, subtitle) {}

/**
 * @brief Inicializar o serviço.
 *
 * Método responsavel pela inicialização dos serviços, caracteristicas e descritores BLE.
 */
void PotentiometerService::init()
{
  Serial.print("Criando um PotentiometerService com o titulo: " + String(getTitle().c_str()) + "... ");

  // Cria um servidor BLE, caso o mesmo já não tenha sido criado.
  EasyBLE::createServer();

  // Cria um serviço BLE, único para cada modulo de serviço implementado.
  BLEService *pService = EasyBLE::createService(getTitle(), getSubtitle());

  // Cria uma caracteristica para atualização dos valores de estado exibidos no aplicativo.
  _pCharacteristicValue = EasyBLE::createCharacteristic(
      pService,
      "POTENTIOMETER Value (%)",
      "Taxa de resistencia do potenciômetro",
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
void PotentiometerService::update()
{
  if (isReady())
  {
    setState(map(analogRead(getPin()), 0, 4095, 0, 100));
    publishState(_pCharacteristicValue);
  }
}
