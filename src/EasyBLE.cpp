#include "EasyBLE.hpp"
#include "EasyBLEServerCallback.hpp"

/**
 * @brief Declaração das variaveis estaticas do modulo.
 *
 */
unsigned short EasyBLE::_nUuids;
bool EasyBLE::_deviceConnected;
BLEServer *EasyBLE::_pServer;

/**
 * @brief Protótipos das funções auxiliares do modulo.
 *
 */
std::string checkText(std::string text);
void addCharacteristicDetails(BLECharacteristic *pCharacteristic, std::string name, std::string description);

/**
 * @brief Criar o servidor BLE.
 *
 * Método responsavel por criar e iniciar um servidor BLE. Apenas um servidor é criado e iniciado.
 * @return Uma referencia para o objeto do servidor BLE.
 */
BLEServer *EasyBLE::createServer()
{
  if (EasyBLE::_pServer == NULL)
  {
    EasyBLE::_nUuids = 0;

    BLEDevice::init("EasyBLE");

    EasyBLE::_pServer = BLEDevice::createServer();
    EasyBLE::_pServer->setCallbacks(new EasyBLEServerCallback());

    EasyBLE::_pServer->startAdvertising();
  }
  return EasyBLE::_pServer;
}

/**
 * @brief Criar um serviço BLE.
 *
 * Método responsavel por criar um serviço BLE, assim como a caracteristica com os detalhes do serviço.
 * @param [in] title A string do titulo do serviço.
 * @param [in] subtitle A string do subtitulo do serviço.
 * @return Uma referencia para o objeto do serviço BLE criado.
 */
BLEService *EasyBLE::createService(std::string title, std::string subtitle)
{
  BLEService *pService = EasyBLE::_pServer->createService(EasyBLE::getNewUUID());
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(EasyBLE::getNewUUID(), BLECharacteristic::PROPERTY_READ);
  addCharacteristicDetails(pCharacteristic, title, subtitle);
  return pService;
}

/**
 * @brief Criar uma caracteristica BLE.
 *
 * Método responsavel por criar uma caracteristica BLE, assim como os detalhes dessa caracteristica. Cada caracteristica deve ter um tipo, que pode ser:
 * INPUT, caso queira que essa caracteristica receba valores de texto do aplicativo;
 * OUTPUT, caso queira escrever valores na caracteristica e notificar os observadores dela; 
 * SWITCH, caso queira receber apenas valores "on" e "off".
 * @param [in] pService A referencia do serviço que receberá a caracteristica.
 * @param [in] title A string do titulo da caracteristica.
 * @param [in] subtitle A string do subtitulo da caracteristica.
 * @param [in] type O tipo de caracteristica. 
 * @param [in] callback A função callback que será acionada a cada escrita dos clientes na caracteristica.
 * @return Uma referencia para o objeto da caracteristica BLE criada.
 */
BLECharacteristic *EasyBLE::createCharacteristic(BLEService *pService, std::string title, std::string subtitle, unsigned char type, EasyBLECharacteristicCallback *callback)
{
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(EasyBLE::getNewUUID(), 0);
  addCharacteristicDetails(pCharacteristic, title, subtitle);

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

/**
 * @brief Está conectado.
 *
 * Método que verifica a existencia de um cliente conectado ao dispositivo bluetooth. Pois, caso não haja, não há necessidade de enviar e receber valores.
 * @return Um booleano informando se há algum cliente conectado ao dispositivo. TRUE quando houver e FALSE caso contrario.
 */
bool EasyBLE::isConnected()
{
  return EasyBLE::_deviceConnected;
}

/**
 * @brief Mudar estado da conexão.
 *
 * Método responsavel por manter atualizado o atual estado de conexão do dispositivo BLE. Ou seja, o dispositivo entra no estado de 
 * conectado ao estabelecer uma conexão com algum cliente e desconectado quando não há mais clientes conectados ao dispositivo.
 * @param [in] newConnection O novo estado de conexão. TRUE significa que um usuario se conectou e FALSE que se desconectou.
 */
void EasyBLE::changeConnection(bool newConnection)
{
  if (newConnection)
  {
    Serial.println("Um dispositivo foi conectado");
    EasyBLE::_deviceConnected = true;
  }
  else
  {
    Serial.println("Um dispositivo foi desconectado");
    if (EasyBLE::_pServer->getConnectedCount() == 0)
    {
      EasyBLE::_deviceConnected = false;
    }
  }
}

/**
 * @brief Ler valor da caracteristica.
 *
 * Método responsavel por ler valores contidos em caracteristicas. É necessário que o dispositivo BLE esteja devidamente iniciado e conectado a algum cliente.
 * @return Uma string contendo o valor lido na caracteristica.
 */
std::string EasyBLE::readValue(BLECharacteristic *pCharacteristic)
{
  if (EasyBLE::isConnected())
  {
    std::string value;
    value = pCharacteristic->getValue();
    return value;
  }
  return NULL;
}

/**
 * @brief Escrever valor na caracteristica e notificar observadores.
 *
 * Método responsavel por escrever valores em caracteristicas. O método faz uso do checkText para que não haja problemas com o dispositivo BLE, 
 * portanto, evite ultrapassar os 100 caracteres. É necessário que o dispositivo BLE esteja devidamente iniciado e conectado a algum cliente.
 * @param [in] pCharacteristic A referencia da caracteristica que receberá a escrita de valor.
 * @param [in] value A string a ser escrita.
 */
void EasyBLE::writeValue(BLECharacteristic *pCharacteristic, std::string value)
{
  if (EasyBLE::isConnected())
  {
    //EasyBLE::_mutex.lock();
    pCharacteristic->setValue(checkText(value));
    pCharacteristic->notify();
    //EasyBLE::_mutex.unlock();
  }
}

/**
 * @brief Obter novo UUID.
 *
 * Método responsavel por criar um UUID segundo a quantidade de UUIDs já criadas.
 * @return A nova UUID.
 */
BLEUUID EasyBLE::getNewUUID()
{
  return BLEUUID(++EasyBLE::_nUuids);
}

/**
 * @brief Checar texto.
 *
 * Função responsavel checar e reduzir strings muito longas. Os descritores BLE não suportam valores com mais de 103 caracteres. 
 * Por garantia, apenas os 100 primeiros caracteres são considerados em cada string.
 * @param [in] text A string a ser chegada.
 * @return A string resultante da checagem.
 */
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

/**
 * @brief Adicionar detalhes a caracteristica.
 *
 * Função responsavel por criar os detalhes de uma caracteristica por meio de descritores. É criado um descritor por detalhe e 
 * cada detalhe é armazenado no valor desse descritor.
 * @param [in] pCharacteristic A referencia da caracteristica que serão adicionados os detalhes.
 * @param [in] title A string do titulo da caracteristica.
 * @param [in] subtitle A string do subtitulo da caracteristica.
 */
void addCharacteristicDetails(BLECharacteristic *pCharacteristic, std::string title, std::string subtitle)
{
  std::string values[] = {title, subtitle};

  for (std::string value : values)
  {
    BLEDescriptor *pDescriptor = new BLEDescriptor(EasyBLE::getNewUUID());
    pDescriptor->setValue(checkText(value));
    pCharacteristic->addDescriptor(pDescriptor);
  }
}