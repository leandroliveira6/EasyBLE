#include "OledService.hpp"

/**
 * @brief Protótipos das funções auxiliares e callbacks do serviço.
 * 
 */
void displayInputCallback(void *pObject, BLECharacteristic *pCharacteristic);
void displaySwitchCallback(void *pObject, BLECharacteristic *pCharacteristic);
SSD1306 *initDisplay(unsigned char sda, unsigned char scl);

/**
 * @brief Construtores do OledService.
 *
 * Serviço de monitoramento e controle de um servomotor pelo aplicativo EasyBLE.
 * @param [in] pin Parametro não utilizado nesse serviço.
 * @param [in] period Periodo (ms) de atualização das leituras e escritas. É opcional e possui como padrão 1000ms.
 * @param [in] title Titulo do serviço, exibido nos cards de serviço no aplicativo. É opcional e possui como padrão "Titulo não definido".
 * @param [in] subtitle Subtitulo do serviço, exibido nos cards de serviço no aplicativo. É opcional e possui como padrão "Subtitulo não definido".
 */
OledService::OledService() : OledService::OledService(DEFAULT_PERIOD, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
OledService::OledService(unsigned int period) : OledService::OledService(period, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
OledService::OledService(std::string title, std::string subtitle) : OledService::OledService(DEFAULT_PERIOD, title, subtitle) {}
OledService::OledService(unsigned int period, std::string title, std::string subtitle) : ServiceBase::ServiceBase(DEFAULT_PIN, period, title, subtitle) {}

/**
 * @brief Inicializar o serviço.
 *
 * Método responsavel pela inicialização dos serviços, caracteristicas e descritores BLE, além de inicializar o componente.
 */
void OledService::init()
{
  Serial.print("Criando um OledService com o titulo: " + String(getTitle().c_str()) + "... ");

  // Configura o componente do serviço e inicializa o estado.
  _display = initDisplay(SDA_PIN, SCL_PIN);
  setState("EasyBLE");

  // Cria um servidor BLE, caso o mesmo já não tenha sido criado.
  EasyBLE::createServer();

  // Cria um serviço BLE, único para cada modulo de serviço implementado.
  BLEService *pService = EasyBLE::createService(getTitle(), getSubtitle());

  // Cria uma caracteristica para atualização dos valores de estado exibidos no aplicativo.
  _pCharacteristicState = EasyBLE::createCharacteristic(
      pService,
      "DISPLAY State",
      "Exibição do estado atual do display",
      EasyBLE::PROPERTY_OUTPUT,
      NULL);

  // Cria uma caracteristica para controle dos valores de estado do serviço.
  EasyBLE::createCharacteristic(
      pService,
      "DISPLAY Input",
      "Envia dados para serem exibidos no display",
      EasyBLE::PROPERTY_INPUT,
      new EasyBLECharacteristicCallback(this, displayInputCallback));

  // Cria uma caracteristica para controle dos valores de estado do serviço.
  EasyBLE::createCharacteristic(
      pService,
      "DISPLAY Cores Invertidas",
      "Liga e desliga as cores invertidas no display",
      EasyBLE::PROPERTY_SWITCH,
      new EasyBLECharacteristicCallback(this, displaySwitchCallback));

  // Inicia o servidor BLE
  pService->start();

  Serial.println("criado.");
};

/**
 * @brief Atualizar o serviço.
 *
 * Método responsavel pela atualização do estado no aplicativo periodicamente.
 */
void OledService::update()
{
  if (isReady(true))
  {
    publishState(_pCharacteristicState);
    setDisplay(getState());
  }
}

/**
 * @brief Configurar valores opcionais do serviço.
 *
 * Método responsavel pelo ajuste das portas do barramento serial.
 * @param [in] sda A porta de dados seriais.
 * @param [in] scl A porta de clock serial.
 */
void OledService::setOptionals(unsigned char sda, unsigned char scl)
{
  _sda = sda;
  _scl = scl;
  _display = initDisplay(_sda, _scl);
}

/**
 * @brief Obter a referencia do display.
 *
 * Método para obtenção da referencia do display utilizado pelo serviço.
 * @return Uma referencia para a instancia do display.
 */
SSD1306 *OledService::getDisplay()
{
  return _display;
}

/**
 * @brief Atualizar display.
 *
 * Função que executa uma escrita no display oled.
 * @param [in] msg Mensagem a ser escrita no display.
 */
void OledService::setDisplay(String msg)
{
  _display->clear();
  _display->drawString(0, 0, msg);
  _display->display();
}

/**
 * @brief Função callback para a caracteristica de input do display.
 *
 * Função callback acionada a cada nova escrita na caracteristica que a contiver. Alem de receber o conteúdo escrito, tambem atualiza o estado do serviço.
 * @param [in] pObject Referencia do serviço da caracteristica.
 * @param [in] pCharacteristic Referencia da caracteristica que recebeu a escrita.
 */
void displayInputCallback(void *pObject, BLECharacteristic *pCharacteristic)
{
  OledService *pOledService = (OledService *)pObject;
  String value = EasyBLE::readValue(pCharacteristic).c_str();

  if (value.length() > 0)
  {
    Serial.println("Valor recebido: " + value);
    pOledService->setState(value);
  }
}

/**
 * @brief Função callback para a caracteristica de movimentação dos dados do display.
 *
 * Função callback acionada a cada nova escrita na caracteristica que a contiver. Alem de receber o conteúdo escrito, tambem atualiza o estado do serviço.
 * @param [in] pObject Referencia do serviço da caracteristica.
 * @param [in] pCharacteristic Referencia da caracteristica que recebeu a escrita.
 */
void displaySwitchCallback(void *pObject, BLECharacteristic *pCharacteristic)
{
  OledService *pOledService = (OledService *)pObject;
  String value = EasyBLE::readValue(pCharacteristic).c_str();

  if (value.length() > 0)
  {
    Serial.println("Valor recebido: " + value);
    if (value == "on")
    {
      pOledService->getDisplay()->invertDisplay();
    }
    else
    {
      pOledService->getDisplay()->normalDisplay();
    }
    pOledService->getDisplay()->display();
  }
}

/**
 * @brief Iniciar display.
 *
 * Função de inicialização do display das portas do barramento serial.
 * @param [in] sda A porta de dados seriais.
 * @param [in] scl A porta de clock serial.
 * @return Uma referencia para a instancia do display.
 */
SSD1306 *initDisplay(unsigned char sda, unsigned char scl)
{
  SSD1306 *display = new SSD1306(0x3C, sda, scl);
  display->init();
  display->flipScreenVertically();
  display->setFont(ArialMT_Plain_16);
  return display;
}