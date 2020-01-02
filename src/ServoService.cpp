#include "ServoService.hpp"

/**
 * @brief Protótipo da função callback do serviço.
 * 
 */
void servoControlCallback(void *pObject, BLECharacteristic *pCharacteristic);

/**
 * @brief Construtores de ServoService.
 *
 * Serviço de monitoramento e controle de um servomotor pelo aplicativo EasyBLE.
 * @param [in] pin Pino onde o componente está conectado. Todos são compativeis, mas apenas os pinos 2,4,12-19,21-23,25-27,32-33 são recomendados.
 * @param [in] period Periodo (ms) de atualização das leituras e escritas. É opcional e possui como padrão 1000ms.
 * @param [in] title Titulo do serviço, exibido nos cards de serviço no aplicativo. É opcional e possui como padrão "Titulo não definido".
 * @param [in] subtitle Subtitulo do serviço, exibido nos cards de serviço no aplicativo. É opcional e possui como padrão "Subtitulo não definido".
 */
ServoService::ServoService(unsigned char pin) : ServoService::ServoService(pin, DEFAULT_PERIOD, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ServoService::ServoService(unsigned char pin, unsigned int period) : ServoService::ServoService(pin, period, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ServoService::ServoService(unsigned char pin, std::string title, std::string subtitle) : ServoService::ServoService(pin, DEFAULT_PERIOD, title, subtitle) {}
ServoService::ServoService(unsigned char pin, unsigned int period, std::string title, std::string subtitle) : ServiceBase::ServiceBase(pin, period, title, subtitle) {}

/**
 * @brief Inicializar o serviço.
 *
 * Método responsavel pela inicialização dos serviços, caracteristicas e descritores BLE, além de inicializar o componente. 
 * Outras configurações podem ser feitas pelo método setOptionals.
 */
void ServoService::init()
{
  Serial.println("Criando o serviço " + String(getTitle().c_str()) + "... ");

  // Configura uma instancia de uma biblioteca externa de controle de servomotores.
  _servo.setPeriodHertz(10); // 10Hz
  _servo.attach(getPin(), DEFAULT_MIN_PULSE, DEFAULT_MAX_PULSE);

  // Cria um servidor BLE, caso o mesmo já não tenha sido criado.
  EasyBLE::createServer();

  // Cria um serviço BLE, único para cada modulo de serviço implementado.
  BLEService *pService = EasyBLE::createService(getTitle(), getSubtitle());

  // Cria uma caracteristica para atualização dos valores de estado exibidos no aplicativo.
  _pCharacteristicValue = EasyBLE::createCharacteristic(
      pService,
      "SERVO State",
      "Exibição do angulo atual do servo",
      EasyBLE::PROPERTY_OUTPUT,
      NULL);

  // Cria uma caracteristica para controle dos valores de estado do serviço.
  EasyBLE::createCharacteristic(
      pService,
      "SERVO Control",
      "Controle do angulo do servomotor. Os angulos estão limitados entre 0 e 180 graus",
      EasyBLE::PROPERTY_INPUT,
      new EasyBLECharacteristicCallback(this, servoControlCallback));

  // Inicia o servidor BLE
  pService->start();

  // Inicializa o estado do serviço.
  setState(0);

  Serial.println("criado.");
};

/**
 * @brief Atualizar o serviço.
 *
 * Método responsavel pela atualização do estado no serviço e no aplicativo, periodicamente, alem de atualizar o estado no componente.
 */
void ServoService::update()
{
  if (isReady(true))
  {
    publishState(_pCharacteristicValue);
    _servo.write(getState());
  }
};

/**
 * @brief Configurar valores opcionais do serviço.
 *
 * Método responsavel pelo ajuste das variáveis de medição e cálculo dos valores do componente.
 * @param [in] minPulseWidth Tamanho minimo do pulso. O valor padrão é 500.
 * @param [in] maxPulseWidth Tamanho maximo do pulso. O valor padrão é 2400.
 */
void ServoService::setOptionals(unsigned short minPulseWidth, unsigned short maxPulseWidth)
{
  _servo.attach(getPin(), minPulseWidth, maxPulseWidth);
}

/**
 * @brief Obter a caracteristica de escrita de valores.
 *
 * Método para obtenção de uma referencia da caracteristica BLE que possibilita atualizar o estado do serviço no aplicativo.
 * @return Uma referencia para o objeto da caracteristica BLE.
 */
BLECharacteristic *ServoService::getCharacteristicValue()
{
  return _pCharacteristicValue;
}

/**
 * @brief Função callback para a caracteristica de controle do servomotor.
 *
 * Função callback acionada a cada nova escrita na caracteristica que a contiver. Alem de receber o conteúdo escrito, tambem atualiza o estado do serviço.
 * @param [in] pObject Referencia do serviço da caracteristica.
 * @param [in] pCharacteristic Referencia da caracteristica que recebeu a escrita.
 */
void servoControlCallback(void *pObject, BLECharacteristic *pCharacteristic)
{
  ServoService *pServoService = (ServoService *)pObject;
  std::string value = EasyBLE::readValue(pCharacteristic);

  if (value.length() > 0)
  {
    Serial.println("Valor recebido: " + String(value.c_str()));
    pServoService->setState(constrain(String(value.c_str()).toInt(), 0, 180));
  }
}
