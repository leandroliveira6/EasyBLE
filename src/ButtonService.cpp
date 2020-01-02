#include "ButtonService.hpp"

void buttonControlCallback(void *pObject, BLECharacteristic *pCharacteristic);
void IRAM_ATTR buttonCallback(void *pObject);

/**
 * @brief Construtores de ButtonService.
 *
 * Serviço de monitoramento e controle de um contador de clicks de um botão pelo aplicativo EasyBLE.
 * @param [in] pin Pino onde o componente está conectado. Apenas os pinos digitais são compativeis.
 * @param [in] period Periodo (ms) de atualização das leituras e escritas. É opcional e possui como padrão 1000ms.
 * @param [in] title Titulo do serviço, exibido nos cards de serviço no aplicativo. É opcional e possui como padrão "Titulo não definido".
 * @param [in] subtitle Subtitulo do serviço, exibido nos cards de serviço no aplicativo. É opcional e possui como padrão "Subtitulo não definido".
 */
ButtonService::ButtonService(unsigned char pin) : ButtonService::ButtonService(pin, DEFAULT_PERIOD, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ButtonService::ButtonService(unsigned char pin, unsigned int period) : ButtonService::ButtonService(pin, period, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ButtonService::ButtonService(unsigned char pin, std::string title, std::string subtitle) : ButtonService::ButtonService(pin, DEFAULT_PERIOD, title, subtitle) {}
ButtonService::ButtonService(unsigned char pin, unsigned int period, std::string title, std::string subtitle) : ServiceBase::ServiceBase(pin, period, title, subtitle) {}

/**
 * @brief Inicializa o componente e todas as instancias BLE.
 *
 * Método responsavel pela inicialização dos serviços, caracteristicas e descritores BLE, além de inicializar o componente.
 */
void ButtonService::init()
{
  Serial.println("Criando o serviço " + String(getTitle().c_str()) + "... ");

  // Configura o pino do serviço para para receber eventos de entrada.
  gpio_set_direction((gpio_num_t)getPin(), GPIO_MODE_INPUT); // Modo de entrada
  gpio_set_intr_type((gpio_num_t)getPin(), GPIO_INTR_NEGEDGE); // Falling edge
  gpio_install_isr_service(0);
  gpio_isr_handler_add((gpio_num_t)getPin(), buttonCallback, this); // Adiciona um callback para eventos no pino

  // Cria um servidor BLE, caso o mesmo já não tenha sido criado.
  EasyBLE::createServer();

  // Cria um serviço BLE, único para cada modulo de serviço implementado.
  BLEService *pService = EasyBLE::createService(getTitle(), getSubtitle());

  // Cria uma caracteristica para atualização dos valores de estado exibidos no aplicativo.
  _pCharacteristicValue = EasyBLE::createCharacteristic(
      pService,
      "BUTTON Click Count",
      "Exibe a quantidade de clicks no botão",
      EasyBLE::PROPERTY_OUTPUT,
      NULL);

  // Cria uma caracteristica para controle dos valores de estado do serviço.
  EasyBLE::createCharacteristic(
      pService,
      "BUTTON Click Control",
      "Controle do número de clicks exibido",
      EasyBLE::PROPERTY_INPUT,
      new EasyBLECharacteristicCallback(this, buttonControlCallback));

  // Inicia o servidor BLE
  pService->start();

  // Inicializa o estado do serviço.
  setState(0);

  Serial.println("criado.");
};

/**
 * @brief Atualiza o estado atual do serviço.
 *
 * Método responsavel pela atualização do estado no aplicativo periodicamente.
 */
void ButtonService::update()
{
  if (isReady())
  {
    publishState(_pCharacteristicValue);
  }
}

/**
 * @brief Obtem uma referencia da caracteristica BLE de atualização do estado do serviço.
 *
 * Método para obtenção de uma referencia da caracteristica BLE que possibilita atualizar o estado do serviço no aplicativo.
 */
BLECharacteristic *ButtonService::getCharacteristicValue()
{
  return _pCharacteristicValue;
}

/**
 * @brief Função callback para a caracteristica de controle do button.
 *
 * Função callback acionada a cada nova escrita na caracteristica que a contiver. Alem de receber o conteúdo escrito, tambem atualiza o estado no serviço e no aplicativo.
 * @param [in] pObject Referencia do serviço da caracteristica.
 * @param [in] pCallback Função callback chamada a cada escrita na caracteristica.
 */
void buttonControlCallback(void *pObject, BLECharacteristic *pCharacteristic)
{
  ButtonService *pButtonService = (ButtonService *)pObject;
  std::string value = EasyBLE::readValue(pCharacteristic);

  if (value.length() > 0)
  {
    Serial.println("Valor recebido: " + String(value.c_str()));
    pButtonService->setState(String(value.c_str()).toInt());
    pButtonService->publishState(pButtonService->getCharacteristicValue());
  }
}

/**
 * @brief Função callback para o evento de clique do botão.
 *
 * Função callback acionada a cada novo clique do botão do serviço. Tem como objetivo atualizar o estado no serviço e no aplicativo.
 * @param [in] pObject Referencia do serviço da caracteristica.
 */
void IRAM_ATTR buttonCallback(void *pObject)
{
  ButtonService *pButtonService = (ButtonService *)pObject;
  int state = pButtonService->getState();
  pButtonService->setState(++state);
  pButtonService->publishState(pButtonService->getCharacteristicValue());
}
