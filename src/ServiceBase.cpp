#ifndef SERVICE_BASE
#define SERVICE_BASE

#include "EasyBLE.hpp"

#define DEFAULT_PERIOD 1000
#define DEFAULT_TITLE "Titulo não definido"
#define DEFAULT_SUBTITLE "Subtitulo não definido"

/**
 * @brief Modelo do ServiceBase.
 *
 */
template <class T>
class ServiceBase
{
public:
  ServiceBase(unsigned char pin, unsigned int period, std::string name, std::string description);
  virtual void init() = 0;
  virtual void update() = 0;
  unsigned char getPin();
  std::string getTitle();
  std::string getSubtitle();
  T getState();
  void setState(T newState);
  bool isReady(bool priority = false);
  void publishState(BLECharacteristic *pCharacteristic);

private:
  unsigned char _pin;
  std::string _title, _subtitle;
  unsigned int _period;
  unsigned long _lastMillis;
  T _state;
  T _lastState;
};

/**
 * @brief Construtor do ServoBase.
 *
 * Serviço que provê métodos comuns para todos os serviços da biblioteca. Armazena dados basicos de cada serviço e 
 * controla tanto os estados quanto o periodo de atualização dos serviços.
 * @param [in] pin Pino onde o componente está conectado.
 * @param [in] period Periodo (ms) de atualização das leituras e escritas.
 * @param [in] title Titulo do serviço, exibido nos cards de serviço no aplicativo.
 * @param [in] subtitle Subtitulo do serviço, exibido nos cards de serviço no aplicativo.
 */
template <class T>
ServiceBase<T>::ServiceBase(unsigned char pin, unsigned int period, std::string title, std::string subtitle)
{
  _pin = pin;
  _title = title;
  _subtitle = subtitle;
  _period = period;
  _lastMillis = millis();
}

/**
 * @brief Iniciar o serviço.
 *
 * Método responsavel pela inicialização do serviço e das caracteristicas GATT. 
 * É nele, também, que se inicializa os componentes eletrônicos.
 */
template <class T>
void ServiceBase<T>::init() {}

/**
 * @brief Atualizar o serviço.
 *
 * Método responsavel pela atualização do serviço e caracteristicas GATT. 
 * É nele, também, que se atualiza o estado dos componentes eletrônicos.
 */
template <class T>
void ServiceBase<T>::update() {}

/**
 * @brief Obter o pino do serviço.
 *
 * Método responsavel pela obtenção do pino do serviço.
 * @return O numero do pino do serviço.
 */
template <class T>
unsigned char ServiceBase<T>::getPin()
{
  return _pin;
}

/**
 * @brief Obter o titulo do serviço.
 *
 * Método responsavel pela obtenção do titulo do serviço.
 * @return A string contendo o titulo do serviço.
 */
template <class T>
std::string ServiceBase<T>::getTitle()
{
  return _title;
}

/**
 * @brief Obter o subtitulo do serviço.
 *
 * Método responsavel pela obtenção do subtitulo do serviço.
 * @return A string contendo o subtitulo do serviço.
 */
template <class T>
std::string ServiceBase<T>::getSubtitle()
{
  return _subtitle;
}

/**
 * @brief Obter o estado do serviço.
 *
 * Método responsavel pela obtenção do estado do serviço.
 * @return O valor do estado do serviço.
 */
template <class T>
T ServiceBase<T>::getState()
{
  return _state;
}

/**
 * @brief Atualizar o estado do serviço.
 *
 * Método responsavel por manter atualizado os dois ultimos estados mais recentes.
 * @param [in] newState O novo estado do serviço.
 */
template <class T>
void ServiceBase<T>::setState(T newState)
{
  _lastState = _state;
  _state = newState;
}

/**
 * @brief Está pronto.
 *
 * Método responsavel por verificar se o periodo de atualização já foi cumprido e se os dois ultimos estados atualizados no 
 * serviço são diferentes. Caso o usuario queira que o periodo de atualização seja sobreposto por mudanças de estados recentes, 
 * precisará informar que a checagem é prioritária com o valor TRUE.
 * @param [in] priority A prioridade da checagem.
 * @return Um booleano informando se o serviço está pronto.
 */
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

/**
 * @brief Publicar o estado.
 *
 * Método responsavel por escrever na caracteristica recebida por parametro o valor de estado mais atual. 
 * Compativel com todos os tipos primitivos da linguagem C++, exceto std::string.
 * @param [in] pCharacteristic A referencia para a caracteristica onde o valor será escrito.
 */
template <class T>
void ServiceBase<T>::publishState(BLECharacteristic *pCharacteristic)
{
  EasyBLE::writeValue(pCharacteristic, String(_state).c_str());
}

#endif