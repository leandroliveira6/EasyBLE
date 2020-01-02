#include "ReleService.hpp"

/**
 * @brief Protótipo da função callback do serviço.
 * 
 */
void releControlCallback(void *pObject, BLECharacteristic *pCharacteristic);

/**
 * @brief Construtores de ReleService.
 *
 * Serviço de monitoramento de um thermistor pelo aplicativo EasyBLE.
 * @param [in] pin Pino onde o componente está conectado. Apenas os pinos digitais são compativeis.
 * @param [in] period Periodo (ms) de atualização das leituras e escritas. É opcional e possui como padrão 1000ms.
 * @param [in] title Titulo do serviço, exibido nos cards de serviço no aplicativo. É opcional e possui como padrão "Titulo não definido".
 * @param [in] subtitle Subtitulo do serviço, exibido nos cards de serviço no aplicativo. É opcional e possui como padrão "Subtitulo não definido".
 */
ReleService::ReleService(unsigned char pin) : ReleService::ReleService(pin, DEFAULT_PERIOD, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ReleService::ReleService(unsigned char pin, unsigned int period) : ReleService::ReleService(pin, period, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ReleService::ReleService(unsigned char pin, std::string title, std::string subtitle) : ReleService::ReleService(pin, DEFAULT_PERIOD, title, subtitle) {}
ReleService::ReleService(unsigned char pin, unsigned int period, std::string title, std::string subtitle) : ServiceBase::ServiceBase(pin, period, title, subtitle) {}

/**
 * @brief Inicializar o serviço.
 *
 * Método responsavel pela inicialização dos serviços, caracteristicas e descritores BLE, além de inicializar o componente.
 */
void ReleService::init()
{
  Serial.println("Criando o serviço " + String(getTitle().c_str()) + "... ");
  // Configura o pino do serviço para saída
  pinMode(getPin(), OUTPUT);

  // Cria um servidor BLE, caso o mesmo já não tenha sido criado.
  EasyBLE::createServer();

  // Cria um serviço BLE, único para cada modulo de serviço implementado.
  BLEService *pService = EasyBLE::createService(getTitle(), getSubtitle());

  // Cria uma caracteristica para atualização dos valores de estado exibidos no aplicativo.
  _pCharacteristicValue = EasyBLE::createCharacteristic(
      pService,
      "RELÉ State",
      "Exibição do estado do relé",
      EasyBLE::PROPERTY_OUTPUT,
      NULL);

  // Cria uma caracteristica para atualização dos valores de estado exibidos no aplicativo.
  EasyBLE::createCharacteristic(
      pService,
      "RELÉ Control",
      "Controle do estado do relé",
      EasyBLE::PROPERTY_SWITCH,
      new EasyBLECharacteristicCallback(this, releControlCallback));

  // Inicia o servidor BLE
  pService->start();

  // Inicializa o estado do serviço.
  setState(STATE_OFF);

  Serial.println("criado.");
};

/**
 * @brief Atualizar o serviço.
 *
 * Método responsavel pela atualização do estado no serviço e no aplicativo, periodicamente, alem de atualizar o estado no componente.
 */
void ReleService::update()
{
  if (isReady(true))
  {
    publishState(_pCharacteristicValue);
    digitalWrite(getPin(), getState() == STATE_ON ? HIGH : LOW);
  }
};

/**
 * @brief Obter a caracteristica de escrita de valores.
 *
 * Método para obtenção de uma referencia da caracteristica BLE que possibilita atualizar o estado do serviço no aplicativo.
 * @return Uma referencia para o objeto da caracteristica BLE.
 */
BLECharacteristic *ReleService::getCharacteristicValue()
{
  return _pCharacteristicValue;
}

/**
 * @brief Função callback para a caracteristica de controle do relé.
 *
 * Função callback acionada a cada nova escrita na caracteristica que a contiver. Alem de receber o conteúdo escrito, tambem atualiza o estado do serviço.
 * @param [in] pObject Referencia do serviço da caracteristica.
 * @param [in] pCharacteristic Referencia da caracteristica que recebeu a escrita.
 */
void releControlCallback(void *pObject, BLECharacteristic *pCharacteristic)
{
  ReleService *pReleService = (ReleService *)pObject;
  std::string value = EasyBLE::readValue(pCharacteristic);

  if (value.length() > 0)
  {
    Serial.println("Valor recebido: " + String(value.c_str()));
    pReleService->setState(value == "on" ? STATE_ON : STATE_OFF);
    pReleService->publishState(pReleService->getCharacteristicValue());
  }
}
