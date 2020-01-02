#include "ThermistorService.hpp"

/**
 * @brief Construtores de ThermistorService.
 *
 * Serviço de monitoramento de um thermistor pelo aplicativo EasyBLE.
 * @param [in] pin Pino onde o componente está conectado. Apenas os pinos analógicos são compativeis.
 * @param [in] period Periodo (ms) de atualização das leituras e escritas. É opcional e possui como padrão 1000ms.
 * @param [in] title Titulo do serviço, exibido nos cards de serviço no aplicativo. É opcional e possui como padrão "Titulo não definido".
 * @param [in] subtitle Subtitulo do serviço, exibido nos cards de serviço no aplicativo. É opcional e possui como padrão "Subtitulo não definido".
 */
ThermistorService::ThermistorService(unsigned char pin) : ThermistorService::ThermistorService(pin, DEFAULT_PERIOD, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ThermistorService::ThermistorService(unsigned char pin, unsigned int period) : ThermistorService::ThermistorService(pin, period, DEFAULT_TITLE, DEFAULT_SUBTITLE) {}
ThermistorService::ThermistorService(unsigned char pin, std::string title, std::string subtitle) : ThermistorService::ThermistorService(pin, DEFAULT_PERIOD, title, subtitle) {}
ThermistorService::ThermistorService(unsigned char pin, unsigned int period, std::string title, std::string subtitle) : ServiceBase::ServiceBase(pin, period, title, subtitle)
{
  _beta = DEFAULT_BETA;
  _maxVoltage = DEFAULT_MAX_VOLTAGE;
  _maxSamples = DEFAULT_MAX_SAMPLES;
  _nSamples = 0;
  _sumSamples = 0;
}

/**
 * @brief Inicializar o serviço.
 *
 * Método responsavel pela inicialização dos serviços, caracteristicas e descritores BLE, além de inicializar o componente e calibrar a entrada.
 */
void ThermistorService::init()
{
  Serial.print("Criando um ThermistorService com o titulo: " + String(getTitle().c_str()) + "... ");

  // Calibragem dos pinos de leitura analogicas para minimizar erros de leitura.
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &_calibration); //Inicializa a estrutura de calibracao

  // Cria um servidor BLE, caso o mesmo já não tenha sido criado.
  EasyBLE::createServer();

  // Cria um serviço BLE, único para cada modulo de serviço implementado.
  BLEService *pService = EasyBLE::createService(getTitle(), getSubtitle());

  // Cria uma caracteristica para atualização dos valores de estado exibidos no aplicativo.
  _pCharacteristicValue = EasyBLE::createCharacteristic(
      pService,
      "THERMISTOR Value (ºC)",
      "Temperatura ambiente",
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
void ThermistorService::update()
{
  if (isReady())
  {
    // 1/Tk = 1/T25 + 1/B*ln(Vin/Vout - 1) [1/K]
    // Tc = 1/(1/Tk)-273.15 [C]
    const float zeroCelciusInKelvin = 273.15;
    _sumSamples += esp_adc_cal_raw_to_voltage(analogRead(ServiceBase<float>::getPin()), &_calibration);
    _nSamples++;
    if (_nSamples == _maxSamples)
    {
      float tInv = 1.0 / (25.0 + zeroCelciusInKelvin) + 2.3 / (float)_beta * log((float)_maxVoltage / ((float)_sumSamples / (float)_maxSamples) - 1.0);
      setState(1.0 / tInv - zeroCelciusInKelvin);
      publishState(_pCharacteristicValue);
      _nSamples = _sumSamples = 0;
    }
  }
}

/**
 * @brief Configurar valores opcionais do serviço.
 *
 * Método responsavel pelo ajuste das variáveis de medição e cálculo dos valores do componente.
 * @param [in] beta Coeficiente beta do thermistor. O valor padrão é 3950.
 * @param [in] maxVoltage Voltagem maxima lida no pino. O valor padrão é 5000 [mV]. 
 * @param [in] maxSamples Numero maximo de amostras para o calculo da temperatura media.
 */
void ThermistorService::setOptionals(unsigned short beta, unsigned short maxVoltage, unsigned short maxSamples)
{
  _beta = beta;
  _maxVoltage = maxVoltage;
  _maxSamples = maxSamples;
}